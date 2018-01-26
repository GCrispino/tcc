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
:txMutacao(c.txMutacao),desvioPadrao(c.desvioPadrao),funcaoFitness(c.funcaoFitness)
{
    this->genotipo = c.genotipo;

    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    if (this->gen == nullptr)
        this->gen = new std::mt19937(seed1);
    this->intDis = std::uniform_int_distribution<int>(0,Cromossomo::N_GENES - 1);
    this->realDis = std::uniform_real_distribution<float>(0,1);
}

Cromossomo& Cromossomo::operator = (const Cromossomo &c){

    if (this == &c)
        return *this;

    this->txMutacao = c.txMutacao;
    this->desvioPadrao = c.desvioPadrao;
    this->fitness = c.fitness;

    this->genotipo = c.genotipo;

    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    if (this->gen == nullptr)
        this->gen = new std::mt19937(seed1);
    this->intDis = std::uniform_int_distribution<int>(0,Cromossomo::N_GENES - 1);
    this->realDis = std::uniform_real_distribution<float>(0,1);

    return *this;
}

void Cromossomo::inicializar(){
    for (double &gene: this->genotipo)
        gene = this->disIntervaloValoresFuncao(*(this->gen));

}

void Cromossomo::calcularFitness() {
    std::vector<double> input{
            this->genotipo[0],this->genotipo[1]
    };

    this->fitness =
            this->funcaoFitness.getFuncao()(input);

}

void Cromossomo::mutacao() {
    double rand;
    for (double &gene: this->genotipo){
        rand= this->gaussianDis(*(this->gen));

        gene += rand;
    }
}

void Cromossomo::mutaFilhos(std::vector<Cromossomo> &filhos) {

    for (Cromossomo &filho : filhos) {
        //double probMutacao = Math.random();
        double probMutacao = this->realDis(*this->gen);/*ALEATORIO!= this->rGen.nextDouble()*/;

        if (probMutacao < filho.txMutacao) {
            filho.mutacao();
        }

    }
}

std::vector<Cromossomo> Cromossomo::crossover(const Cromossomo &outroCromossomo){

    Cromossomo filho1(this->txMutacao,this->desvioPadrao,this->funcaoFitness),
            filho2(this->txMutacao,this->desvioPadrao,this->funcaoFitness);

    double alpha;

    //segunda parte
    for (int i = 0; i < Cromossomo::N_GENES; ++i) {
        alpha = this->realDis(*(this->gen));

        filho1.genotipo[i] = alpha * this->genotipo[i] + (1 - alpha) * outroCromossomo.genotipo[i];
        filho2.genotipo[i] = alpha * outroCromossomo.genotipo[i] + (1 - alpha) * this->genotipo[i];
    }

    filho1.calcularFitness();
    filho2.calcularFitness();

    std::vector<Cromossomo> filhos;

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
    this->mutaFilhos(filhos);


    return filhos;
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