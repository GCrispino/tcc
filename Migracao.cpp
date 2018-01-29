//
// Created by Crispino on 1/27/2018.
//

#include <chrono>
#include <iostream>
#include "Migracao.h"

Migracao::Migracao(double probMigracao,std::vector<Populacao *> &populacoes):probMigracao(probMigracao){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    this->gen = std::mt19937(seed);

    this->populacoes = populacoes;

    this->realDis = std::uniform_real_distribution<float>(0.0,1.0);
    this->intDis = std::uniform_int_distribution<int>(0,this->populacoes.size() - 1);
}

void Migracao::iniciarMigracao(int &nPopulacoesProcessadas){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::uniform_int_distribution<int> popDis(0,this->populacoes[0]->getTamanho() - 1);


    while(nPopulacoesProcessadas < this->populacoes.size()){
        double rand = this->realDis(this->gen);
        if (rand < this->probMigracao) {
            int
                iPopulacao1 = this->intDis(this->gen),
                iPopulacao2 = -1,
                iCromossomoAleatorio1 = popDis(this->gen),
                iCromossomoAleatorio2 = popDis(this->gen);

            do {
                iPopulacao2 = this->intDis(this->gen);
            }while(iPopulacao1 == iPopulacao2);

            /*std::cout << iPopulacao1 << ',';
            std::cout << iPopulacao2 << std::endl;

            std::cout << iCromossomoAleatorio1 << ',';
            std::cout << iCromossomoAleatorio2 << std::endl;*/

            Populacao
                    *populacao1 = this->populacoes[iPopulacao1],
                    *populacao2 = this->populacoes[iPopulacao2];

            Cromossomo
                melhorPopulacao1 = populacao1->getElemMaxFitness(),
                melhorPopulacao2 = populacao2->getElemMaxFitness();

            populacao1->setMomentoMigracao(true);
            populacao2->setMomentoMigracao(true);

            populacao1->setCromossomo(iCromossomoAleatorio1,melhorPopulacao2);
            populacao2->setCromossomo(iCromossomoAleatorio2,melhorPopulacao1);
            std::cout << "";


            populacao1->setMomentoMigracao(false);
            populacao2->setMomentoMigracao(false);

        }
    }
}
