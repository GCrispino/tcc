#include "Cromossomo.hpp"
#include <iostream>
#include <chrono>

std::ostream &operator << (std::ostream &output, const Cromossomo &c){
    output.precision(10);
    output << "Cromossomo: {fitness: "<< c.fitness;
    output << ", txMutacao}" << c.txMutacao << std::endl;

    output << "Genotipo: " << std::endl;

    output << std::endl;

    output << '[';
    for (const double &gene: c.genotipo){
        output << gene << ',';
    }
    output << ']' << std::endl;

    return output;
}

std::mt19937* Cromossomo::gen = nullptr;

Cromossomo::Cromossomo(float txMutacao,float desvioPadrao)
:txMutacao(txMutacao),desvioPadrao(desvioPadrao),funcaoFitness(Funcao())
{
    double
            *intervaloFuncaoFitness = funcaoFitness.getIntervalo(),
            intervaloMin = intervaloFuncaoFitness != nullptr ? intervaloFuncaoFitness[0] : 0,
            intervaloMax = intervaloFuncaoFitness != nullptr ? intervaloFuncaoFitness[1] : 0;

    //=======================================================
    //intervaloMin = -32;
    //intervaloMax = 32;
    //=======================================================

    this->fitness = -1;

    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    if (this->gen == nullptr)
        this->gen = new std::mt19937(seed1);

    this->genotipo = std::vector<double>(Cromossomo::N_GENES);

    this->intDis = std::uniform_int_distribution<int>(0,Cromossomo::N_GENES - 1);
    this->realDis = std::uniform_real_distribution<float>(0,1);
    this->disIntervaloValoresFuncao = std::uniform_real_distribution<double>(intervaloMin,intervaloMax);
    this->gaussianDis = std::normal_distribution<double>(0,this->desvioPadrao);
}

Cromossomo::Cromossomo(float txMutacao,float desvioPadrao,const Funcao &funcaoFitness)
        :txMutacao(txMutacao),desvioPadrao(desvioPadrao),funcaoFitness(funcaoFitness)
{
    double
            *intervaloFuncaoFitness = funcaoFitness.getIntervalo(),
            intervaloMin = intervaloFuncaoFitness != nullptr ? intervaloFuncaoFitness[0] : -100,
            intervaloMax = intervaloFuncaoFitness != nullptr ? intervaloFuncaoFitness[1] : 100;
    this->fitness = -1;

    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    if (this->gen == nullptr)
        this->gen = new std::mt19937(seed1);

    this->genotipo = std::vector<double>(Cromossomo::N_GENES);

    this->intDis = std::uniform_int_distribution<int>(0,Cromossomo::N_GENES - 1);
    this->realDis = std::uniform_real_distribution<float>(0,1);
    this->disIntervaloValoresFuncao = std::uniform_real_distribution<double>(intervaloMin,intervaloMax);
    this->gaussianDis = std::normal_distribution<double>(0,this->desvioPadrao);
}

Cromossomo::Cromossomo(const Cromossomo &c)
:txMutacao(c.txMutacao),desvioPadrao(c.desvioPadrao),fitness(c.fitness),funcaoFitness(c.funcaoFitness)
{
    double
            *intervaloFuncaoFitness = c.funcaoFitness.getIntervalo(),
            intervaloMin = intervaloFuncaoFitness != nullptr ? intervaloFuncaoFitness[0] : 0,
            intervaloMax = intervaloFuncaoFitness != nullptr ? intervaloFuncaoFitness[1] : 0;

    if (!c.genotipo.empty())
        this->genotipo = c.genotipo;

    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    if (this->gen == nullptr)
        this->gen = new std::mt19937(seed1);
    this->intDis = std::uniform_int_distribution<int>(0,Cromossomo::N_GENES - 1);
    this->realDis = std::uniform_real_distribution<float>(0,1);
    this->disIntervaloValoresFuncao = std::uniform_real_distribution<double>(intervaloMin,intervaloMax);
    this->gaussianDis = std::normal_distribution<double>(0,this->desvioPadrao);

}

Cromossomo& Cromossomo::operator = (const Cromossomo &c){

    if (this == &c)
        return *this;

    double
            *intervaloFuncaoFitness = c.funcaoFitness.getIntervalo(),
            intervaloMin = intervaloFuncaoFitness != nullptr ? intervaloFuncaoFitness[0] : 0,
            intervaloMax = intervaloFuncaoFitness != nullptr ? intervaloFuncaoFitness[1] : 0;


    this->txMutacao = c.txMutacao;
    this->desvioPadrao = c.desvioPadrao;
    this->fitness = c.fitness;
    this->funcaoFitness =c.funcaoFitness;


    if (!c.genotipo.empty())
        this->genotipo = c.genotipo;
    else
        this->genotipo = std::vector<double>(Cromossomo::N_GENES);


    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    if (this->gen == nullptr)
        this->gen = new std::mt19937(seed1);
    this->intDis = std::uniform_int_distribution<int>(0,Cromossomo::N_GENES - 1);
    this->realDis = std::uniform_real_distribution<float>(0,1);
    this->disIntervaloValoresFuncao = std::uniform_real_distribution<double>(intervaloMin,intervaloMax);
    this->gaussianDis = std::normal_distribution<double>(0,this->desvioPadrao);


    return *this;
}

void Cromossomo::inicializar(){
    for (double &gene: this->genotipo) {
        gene = this->disIntervaloValoresFuncao(*(this->gen));
        double * intervalo = this->funcaoFitness.getIntervalo();
    }

}

void Cromossomo::calcularFitness() {
    std::vector<double> input{
            this->genotipo[0],this->genotipo[1]
    };

    this->fitness =
            this->funcaoFitness.getFuncao()(input);

}

void Cromossomo::mutacao(unsigned int geracaoAtual, unsigned int nGeracoes,bool calculaProbabilidadeCadaFilho) {
    double *intervalo = this->funcaoFitness.getIntervalo();
    for (double &gene: this->genotipo){
        if (!calculaProbabilidadeCadaFilho || this->realDis(*this->gen) < this->txMutacao) {
            double rand = this->gaussianDis(*(this->gen));
            double novoGene =gene + (1 - 0.9 * ((geracaoAtual - 1) / (nGeracoes - 1))) * rand;
            if(novoGene < intervalo[0] || novoGene > intervalo[1]) {
                continue;
            }
            gene = novoGene;
        }
    }
}


void Cromossomo::mutaFilhos(std::vector<Cromossomo> &filhos,unsigned int geracaoAtual, unsigned int nGeracoes) {
    for (Cromossomo &filho : filhos)
        filho.mutacao(geracaoAtual,nGeracoes,true);
}

std::vector<Cromossomo> Cromossomo::crossover(const Cromossomo &outroCromossomo, unsigned int geracaoAtual, unsigned int nGeracoes){

    Cromossomo filho1(this->txMutacao,this->desvioPadrao,this->funcaoFitness),
            filho2(this->txMutacao,this->desvioPadrao,this->funcaoFitness);

    double *intervalo = this->funcaoFitness.getIntervalo();
    //segunda parte
    for (int i = 0; i < Cromossomo::N_GENES; ++i) {
        double alpha = this->realDis(*(this->gen));

        filho1.genotipo[i] = alpha * this->genotipo[i] + (1 - alpha) * outroCromossomo.genotipo[i];
        if(filho1.genotipo[i] < intervalo[0] || filho1.genotipo[i] > intervalo[1]) {
            filho1.genotipo[i] = this->genotipo[i];
        }
        filho2.genotipo[i] = alpha * outroCromossomo.genotipo[i] + (1 - alpha) * this->genotipo[i];
        if (filho2.genotipo[i] < intervalo[0] || filho2.genotipo[i] > intervalo[1]){
            filho2.genotipo[i] = outroCromossomo.genotipo[i];
        }
    }

    filho1.calcularFitness();
    filho2.calcularFitness();

    std::vector<Cromossomo>
            filhos;

    if (filho1.fitness < filho2.fitness)
        filhos = {
                filho1,
                filho2
        };
    else
        filhos = {
                filho2,
                filho1
        };

    //realiza a opera��o de muta��o probabilisticamente nos filhos
    this->mutaFilhos(filhos, geracaoAtual, nGeracoes);


    return filhos;
}

Cromossomo Cromossomo::crossoverSimples(const Cromossomo &outroCromossomo, unsigned int geracaoAtual, unsigned int nGeracoes){

    Cromossomo filho(this->txMutacao,this->desvioPadrao,this->funcaoFitness);

    double alpha;
    double *intervalo = this->funcaoFitness.getIntervalo();

    for (int i = 0; i < Cromossomo::N_GENES; ++i) {
        alpha = this->realDis(*(this->gen));

        filho.genotipo[i] = alpha * this->genotipo[i] + (1 - alpha) * outroCromossomo.genotipo[i];
        if(filho.genotipo[i] < intervalo[0] || filho.genotipo[i] > intervalo[1]) {
            filho.genotipo[i] = this->genotipo[i];
        }
    }

    filho.calcularFitness();

    double probMutacao = this->realDis(*this->gen);

    if (probMutacao < filho.txMutacao) {
        filho.mutacao(geracaoAtual, nGeracoes,true);
    }


    return filho;
}


double Cromossomo::getFitness() const{
    return this->fitness;
}

void Cromossomo::setFuncao(const Funcao &funcao){
    this->funcaoFitness = funcao;

    double
            *intervaloFuncaoFitness = funcaoFitness.getIntervalo(),
            intervaloMin = intervaloFuncaoFitness != nullptr ? intervaloFuncaoFitness[0] : -100,
            intervaloMax = intervaloFuncaoFitness != nullptr ? intervaloFuncaoFitness[1] : 100;

    this->disIntervaloValoresFuncao = std::uniform_real_distribution<double>(intervaloMin,intervaloMax);
}