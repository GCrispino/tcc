//
// Created by Crispino on 1/27/2018.
//

#ifndef TCC_MIGRACAO_H
#define TCC_MIGRACAO_H


#include <random>
#include <vector>
#include "Populacao.h"

class Migracao {
    private:
        double probMigracao;
        std::mt19937 gen;
        std::uniform_int_distribution<int> intDis;
        std::uniform_real_distribution<float> realDis;
        std::vector<Populacao *> populacoes;


    public:
        Migracao(double probMigracao,std::vector<Populacao *> &populacoes);

        void iniciarMigracao(int &nPopulacoesProcessadas);
        void realizarMigracao(int &nPopulacoesProcessadas);
};


#endif //TCC_MIGRACAO_H
