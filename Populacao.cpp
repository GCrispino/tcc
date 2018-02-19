//
// Created by Crispino on 1/12/2018.
//

#include "Populacao.h"
#include <chrono>
#include <omp.h>
#include <iostream>

/**
 * PROBLEMA
	Cromossomos mortos tem fitness errado
	função calcularFitness() da população só calcula da população dos vivos
	No entanto, isso não deveria ser necessário porque os indivíduos já são transferidos com o fitness calculado para a população dos mortos
 */


std::mt19937* Populacao::gen = nullptr;

Populacao::Populacao(unsigned int tamanho, unsigned int tamanhoPopulacaoMortos,float txMutacao, float txCruzamento,unsigned int taxaInfeccao,float desvioPadrao,const Funcao &funcaoFitness) {
    this->tamanho = tamanho;
    this->tamanhoPopulacaoMortos = tamanhoPopulacaoMortos;
    this->txMutacao = txMutacao;
    this->txCruzamento = txCruzamento;
    this->desvioPadrao = desvioPadrao;
    this->funcaoFitness = funcaoFitness;

    this->acabou = false;
    this->elemMaxFitness = nullptr;
    this->iElemMaxFitness = -1;
    this->iElemMinFitness = -1;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    if (this->gen == nullptr)
        this->gen = new std::mt19937(seed);
}

void Populacao::inicializacao() {
    for (int i = 0; i < this->tamanho; i++) {
        Cromossomo cromossomo(this->txMutacao,this->desvioPadrao,this->funcaoFitness);
        cromossomo.inicializar();
        this->cromossomos.push_back(cromossomo);
    }
}


void Populacao::calcularFitness() {

    double accFitness = 0;

    //#pragma omp parallel for
    for (int i = 0;i < this->tamanho;++i){
        Cromossomo &c = this->cromossomos[i];

        c.calcularFitness();
        double fitness = c.getFitness();
        accFitness += fitness;

        if (this->elemMaxFitness == nullptr || fitness < this->elemMaxFitness->getFitness()) {
            this->elemMaxFitness = &c;
            this->iElemMaxFitness = i;

            if (fitness <= this->funcaoFitness.getMinimoGlobal() + pow(10,-3)) {
                this->acabou = true;
            }
        }

        if (this->iElemMinFitness == -1)
            this->iElemMinFitness = i;
        else if(fitness > this->getElemMinFitness().getFitness())
            this->iElemMinFitness = i;
    }

    this->mediaFitness = accFitness / this->tamanho;
}


std::vector<Cromossomo> Populacao::selecaoPais(int nParesPaisASelecionar,int tamanhoTorneio) {
    std::vector<Cromossomo> pais;

    std::uniform_int_distribution<int> dist (0,this->tamanho - 1);
    std::uniform_real_distribution<double> realDis;


    for (int i = 0;i < nParesPaisASelecionar;++i){

        if (realDis(*(this->gen)) > this->txCruzamento)
            continue;

        for (int j = 0;j < 2;++j) {

            std::vector<Cromossomo> torneio;

            while (torneio.size() < tamanhoTorneio) {
                //std::cout << "oi" << std::endl;
                Cromossomo *rand = nullptr;
                int index;

                index = dist(*this->gen);

                rand = &(this->cromossomos[index]);

                if (std::find_if(torneio.begin(), torneio.end(), [&rand](const Cromossomo &c) { return &c == rand; }) !=
                    torneio.end())
                    continue;

                torneio.push_back(*rand);
            }

            if (torneio[0].getFitness() < torneio[1].getFitness())
                pais.push_back(torneio[0]);
            else
                pais.push_back(torneio[1]);

        }

    }

    return pais;

}

std::vector<Cromossomo> Populacao::gerarFilhos(std::vector<Cromossomo> &paisSelecionados) {
    std::vector<Cromossomo> filhos(paisSelecionados.size());

    for (int i = 0;i < paisSelecionados.size();i += 2){
        Cromossomo
                &pai1 = paisSelecionados[i],
                &pai2 = paisSelecionados[i + 1];

        std::vector<Cromossomo>
                filhosGerados = pai1.crossover(pai2);

        filhos[i] = filhosGerados[0];
        filhos[i + 1] = filhosGerados[1];
    }

    return filhos;
}


void Populacao::selecaoSobreviventes(const std::vector<Cromossomo> &filhos) {
    int indicePior, indiceSegundoPior;

    const Cromossomo pior = this->getElemMinFitness();

    for (int i = 0;i < filhos.size();i += 2){
        const Cromossomo
            &filho1 = filhos[i],
            &filho2 = filhos[i + 1];


        const Cromossomo
                * escolhido = filho1.getFitness() < filho2.getFitness() ? &filho1 : &filho2,
                * outro = escolhido->getFitness() == filho1.getFitness() ? &filho2 : &filho1;

        this->cromossomos[this->iElemMinFitness] = *escolhido;
        this->mortos.push_back(*outro);

        this->iElemMinFitness = this->achaIndicePiorFitness();

    }
    return ;
}

void Populacao::recombinacao(){
    /*
     * Seleciona um aleatório da população
     * Seleciona um aleatório da população dos mortos
     * faz o cruzamento simples
     *      se o filho for melhor, incrementa o valor de fitMorto
     *      se não, decrementa
     * ao fim, se a população estiver cheia, tira os indivíduos com os menores fitMorto até ela alcançar o seu tamanho máximo
     */

    std::uniform_int_distribution<int>
            distPopulacao (0,this->tamanho - 1),
            distMortos (0,this->mortos.size() - 1);

    unsigned int
            iCromossomo = distPopulacao(*(this->gen)),
            iMorto = distMortos(*(this->gen));

    CromossomoMorto &morto = this->mortos[iMorto];

    Cromossomo
        &c = this->cromossomos[iCromossomo],
        novo = c.crossoverSimples(morto);

    if (novo.getFitness() < c.getFitness()){
        //decrementa fitMorto
        morto.decFitMorto();
    }
    else{
        //incrementa fitMorto
        morto.incFitMorto();
    }

    unsigned int tamAtualPopulacaoMortos = this->mortos.size();
    
    if (tamAtualPopulacaoMortos > this->tamanhoPopulacaoMortos){
        //ordenar população baseado no fitmorto
        std::sort(this->mortos.begin(),this->mortos.end(),[](CromossomoMorto &c1, CromossomoMorto &c2){
            return c1.getFitMorto() > c2.getFitMorto();
        });

        for (auto &morto: this->mortos){
            morto.calcularFitness();
        }

        //excluir os menores
        std::vector<CromossomoMorto>::iterator inicioRemocao = this->mortos.begin() + this->tamanhoPopulacaoMortos;
        this->mortos.erase(inicioRemocao,this->mortos.end());
        std::cout << "";
    }

}

bool Populacao::verificarParada() {
    return this->acabou;
}

void Populacao::setAcabou() {
    this->acabou = true;
}

const Cromossomo & Populacao::getElemMaxFitness() {
    return *(this->elemMaxFitness);
}

const Cromossomo & Populacao::getElemMinFitness(){
    return this->cromossomos[this->iElemMinFitness];
}

unsigned int Populacao::achaIndicePiorFitness(){
    unsigned int indicePiorFitness;

    for (int i = 0;i < this->cromossomos.size();++i){
        if (i == 0){
            indicePiorFitness = i;
            continue;
        }

        if (this->cromossomos[i].getFitness() > this->cromossomos[indicePiorFitness].getFitness())
            indicePiorFitness = i;
    }

    return indicePiorFitness;
}

void Populacao::setElemMaxFitness(Cromossomo *c) {
    this->elemMaxFitness = c;
}

double Populacao::getMediaFitness(){
    return this->mediaFitness;
}