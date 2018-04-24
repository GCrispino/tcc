//
// Created by Crispino on 1/12/2018.
//

#include "Populacao.h"
#include <chrono>
#include <omp.h>
#include <iostream>




std::mt19937* Populacao::gen = nullptr;
unsigned int Populacao::count = 0;

Populacao::Populacao(unsigned int tamanho, float txMutacao, float txCruzamento,float desvioPadrao,const Funcao &funcaoFitness,bool paralelo) {
    this->id = ++count;

    this->tamanho = tamanho;
    this->txMutacao = txMutacao;
    this->txCruzamento = txCruzamento;
    this->desvioPadrao = desvioPadrao;
    this->funcaoFitness = funcaoFitness;

    this->acabou = false;
    this->momentoMigracao = false;
    this->ocupada = false;
    this->elemMinFitness = nullptr;
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
    while(this->paralelo && this->momentoMigracao);

    double accFitness = 0;

    //#pragma omp parallel for
    for (int i = 0;i < this->tamanho;++i){
        Cromossomo &c = this->cromossomos[i];

        c.calcularFitness();
        double fitness = c.getFitness();
        accFitness += fitness;

        if (this->elemMinFitness == nullptr || fitness < this->elemMinFitness->getFitness()) {
            this->elemMinFitness = &c;
            this->iElemMinFitness = i;

            if (fitness <= this->funcaoFitness.getMinimoGlobal() + pow(10,-3)) {
                this->acabou = true;
            }
        }

        if (this->iElemMaxFitness == -1)
            this->iElemMaxFitness = i;
        else if(fitness > this->getElemMaxFitness().getFitness())
            this->iElemMaxFitness = i;
    }

    this->mediaFitness = accFitness / this->tamanho;
}


std::vector<Cromossomo> Populacao::selecaoPais(int nParesPaisASelecionar,int tamanhoTorneio) {
    while(this->paralelo && this->momentoMigracao);

    this->ocupada = true;


    std::vector<Cromossomo> pais;

    std::uniform_int_distribution<int> dist (0,this->tamanho - 1);
    std::uniform_real_distribution<double> realDis;


    for (int i = 0;i < nParesPaisASelecionar;++i){

        if (realDis(*(this->gen)) > this->txCruzamento)
            continue;

        std::vector<Cromossomo *> parPais(2);

        for (int j = 0;j < 2;++j) {

            //std::vector<Cromossomo> torneio;
            std::vector<Cromossomo *> torneio;

            while (torneio.size() < tamanhoTorneio) {
                //std::cout << "oi" << std::endl;
                Cromossomo *rand = nullptr;
                int index;

                index = dist(*this->gen);

                rand = &(this->cromossomos[index]);

                bool
                    estaNoTorneio = std::find_if(torneio.begin(), torneio.end(), [&rand](const Cromossomo *c) { return c == rand; }) !=
                                        torneio.end(),
                    estaNoPar = (j == 1) && (parPais[0] == rand);

                if (estaNoTorneio || estaNoPar)
                    continue;

                //torneio.push_back(*rand);
                torneio.push_back(rand);
            }

            if (torneio[0]->getFitness() < torneio[1]->getFitness())
                parPais[j] =torneio[0];
            else
                parPais[j] = torneio[1];
        }
        pais.push_back(*parPais[0]);
        pais.push_back(*parPais[1]);
    }
    this->ocupada = false;

    return pais;

}

std::vector<Cromossomo> Populacao::gerarFilhos(std::vector<Cromossomo> &paisSelecionados, unsigned int geracaoAtual, unsigned int nGeracoes) {
    while(this->paralelo && this->momentoMigracao);

    std::vector<Cromossomo> filhos(paisSelecionados.size());

    for (int i = 0;i < paisSelecionados.size();i += 2){
        Cromossomo
                &pai1 = paisSelecionados[i],
                &pai2 = paisSelecionados[i + 1];

        std::vector<Cromossomo>
                filhosGerados = pai1.crossover(pai2,geracaoAtual,nGeracoes);

        filhos[i] = filhosGerados[0];
        filhos[i + 1] = filhosGerados[1];
    }

    return filhos;
}


void Populacao::selecaoSobreviventes(const std::vector<Cromossomo> &filhos) {
    //if (acabou) return;
    while(this->paralelo && this->momentoMigracao);

    this->ocupada = true;

    const Cromossomo pior = this->getElemMaxFitness();

    for (int i = 0;i < filhos.size();i += 2){
        const Cromossomo
            &filho1 = filhos[i],
            &filho2 = filhos[i + 1];

        const Cromossomo
                * escolhido = filho1.getFitness() < filho2.getFitness() ? &filho1 : &filho2,
                &pior = this->cromossomos[this->iElemMinFitness];

        this->cromossomos[this->iElemMaxFitness] = *escolhido;

        this->iElemMaxFitness = this->achaIndicePiorFitness();

    }


    this->ocupada = false;
}

bool Populacao::verificarParada() {
    return this->acabou;
}

void Populacao::setAcabou() {
    this->acabou = true;
}

const unsigned int Populacao::getID(){
    return this->id;
}

bool Populacao::estaOcupada(){
    return this->ocupada;
}

const Cromossomo & Populacao::getElemMinFitness() {
    return *(this->elemMinFitness);
}

const Cromossomo & Populacao::getElemMaxFitness(){
    return this->cromossomos[this->iElemMaxFitness];
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

void Populacao::setElemMinFitness(Cromossomo *c) {
    this->elemMinFitness = c;
}

double Populacao::getMediaFitness(){
    return this->mediaFitness;
}


int Populacao::getTamanho() const{
    return this->tamanho;
}

const Funcao Populacao::getFuncaoFitness() const{
    return this->funcaoFitness;
}

Cromossomo Populacao::getCromossomo(int i) const{
    return this->cromossomos[i];
}

void Populacao::setCromossomo(int i,Cromossomo &c){
    this->cromossomos[i] = c;
}

void Populacao::setMomentoMigracao(bool m){
    this->momentoMigracao = m;
}