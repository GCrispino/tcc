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

    Populacao
            *populacao1 = nullptr,
            *populacao2 = nullptr;


    while(nPopulacoesProcessadas < this->populacoes.size()){
        double rand = this->realDis(this->gen);
        if (rand < this->probMigracao) {
            int
                iPopulacao1,
                iPopulacao2,
                iCromossomoAleatorio1 = popDis(this->gen),
                iCromossomoAleatorio2 = popDis(this->gen);

            bool acabou,ocupada = false,todasPopulacoesProcessadas;
            do {
                iPopulacao1 = this->intDis(this->gen);

                acabou = this->populacoes[iPopulacao1]->verificarParada();
                ocupada = this->populacoes[iPopulacao1]->estaOcupada();
                todasPopulacoesProcessadas = nPopulacoesProcessadas >= this->populacoes.size();
                if (todasPopulacoesProcessadas)
                    break;
                bool teste =!todasPopulacoesProcessadas && (!acabou && !ocupada);
            //}while(todasPopulacoesProcessadas && !(acabou || ocupada));
            }while(!todasPopulacoesProcessadas && (acabou || ocupada));


            populacao1 = this->populacoes[iPopulacao1];
            populacao1->setMomentoMigracao(true);

            do {
                iPopulacao2 = this->intDis(this->gen);

                acabou = this->populacoes[iPopulacao2]->verificarParada();
                ocupada = this->populacoes[iPopulacao2]->estaOcupada();
                todasPopulacoesProcessadas = nPopulacoesProcessadas >= this->populacoes.size();
                if (acabou)
                  std::cout << "";
            /*}while(
                    todasPopulacoesProcessadas
                    && !(acabou || (iPopulacao1 == iPopulacao2) || ocupada)
            );*/
                /*if (todasPopulacoesProcessadas)
                    break;*/
                bool teste =!todasPopulacoesProcessadas && (!acabou && !ocupada);
            }while(
                    !todasPopulacoesProcessadas
                    && (acabou || (iPopulacao1 == iPopulacao2) || ocupada)
            );

            populacao2 = this->populacoes[iPopulacao2];
            populacao2->setMomentoMigracao(true);


            if (nPopulacoesProcessadas >= this->populacoes.size()) {
                populacao1->setMomentoMigracao(false);
                populacao2->setMomentoMigracao(false);
                return;
            }
            /*std::cout << iPopulacao1 << ',';
            std::cout << iPopulacao2 << std::endl;

            std::cout << iCromossomoAleatorio1 << ',';
            std::cout << iCromossomoAleatorio2 << std::endl;*/

            Cromossomo
                melhorPopulacao1 = populacao1->getElemMaxFitness(),
                melhorPopulacao2 = populacao2->getElemMaxFitness();

            populacao1->setCromossomo(iCromossomoAleatorio1,melhorPopulacao2);
            populacao2->setCromossomo(iCromossomoAleatorio2,melhorPopulacao1);


            populacao1->setMomentoMigracao(false);
            populacao2->setMomentoMigracao(false);

        }

    }
    #pragma omp critical
    std::cout << "Acabou migração" << std::endl;
}

void Migracao::realizarMigracao(int &nPopulacoesProcessadas){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::uniform_int_distribution<int> popDis(0,this->populacoes[0]->getTamanho() - 1);

    Populacao
            *populacao1 = nullptr,
            *populacao2 = nullptr;


    double rand = this->realDis(this->gen);
    if (rand < this->probMigracao) {
        int
                iPopulacao1,
                iPopulacao2,
                iCromossomoAleatorio1 = popDis(this->gen),
                iCromossomoAleatorio2 = popDis(this->gen);

        bool acabou, ocupada = false, todasPopulacoesProcessadas;
        do {
            iPopulacao1 = this->intDis(this->gen);

            acabou = this->populacoes[iPopulacao1]->verificarParada();
            ocupada = this->populacoes[iPopulacao1]->estaOcupada();
            todasPopulacoesProcessadas = nPopulacoesProcessadas >= this->populacoes.size();
            if (todasPopulacoesProcessadas)
                break;
            bool teste = !todasPopulacoesProcessadas && (!acabou && !ocupada);
            //}while(todasPopulacoesProcessadas && !(acabou || ocupada));
        } while (!todasPopulacoesProcessadas && (acabou || ocupada));


        populacao1 = this->populacoes[iPopulacao1];
        populacao1->setMomentoMigracao(true);

        do {
            iPopulacao2 = this->intDis(this->gen);

            acabou = this->populacoes[iPopulacao2]->verificarParada();
            ocupada = this->populacoes[iPopulacao2]->estaOcupada();
            todasPopulacoesProcessadas = nPopulacoesProcessadas >= this->populacoes.size();
            if (acabou)
                std::cout << "";
            /*}while(
                    todasPopulacoesProcessadas
                    && !(acabou || (iPopulacao1 == iPopulacao2) || ocupada)
            );*/
            /*if (todasPopulacoesProcessadas)
                break;*/
            bool teste = !todasPopulacoesProcessadas && (!acabou && !ocupada);
        } while (
                !todasPopulacoesProcessadas
                && (acabou || (iPopulacao1 == iPopulacao2) || ocupada)
                );

        populacao2 = this->populacoes[iPopulacao2];
        populacao2->setMomentoMigracao(true);


        if (nPopulacoesProcessadas >= this->populacoes.size()) {
            populacao1->setMomentoMigracao(false);
            populacao2->setMomentoMigracao(false);
            return;
        }
        /*std::cout << iPopulacao1 << ',';
        std::cout << iPopulacao2 << std::endl;

        std::cout << iCromossomoAleatorio1 << ',';
        std::cout << iCromossomoAleatorio2 << std::endl;*/

        Cromossomo
                melhorPopulacao1 = populacao1->getElemMaxFitness(),
                melhorPopulacao2 = populacao2->getElemMaxFitness();

        populacao1->setCromossomo(iCromossomoAleatorio1, melhorPopulacao2);
        populacao2->setCromossomo(iCromossomoAleatorio2, melhorPopulacao1);


        populacao1->setMomentoMigracao(false);
        populacao2->setMomentoMigracao(false);

    }

    /*#pragma omp critical
    std::cout << "Acabou migração" << std::endl;*/
}
