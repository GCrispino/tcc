#include <iostream>
#include <chrono>
#include <omp.h>
#include "Populacao.h"
#include "Funcoes.hpp"
#include "Migracao.h"

#define N_POPULACOES 10

void inicializarPopulacoes(
        int n,
        std::vector<Populacao *> &populacoes,
        int tamanho,
        double txMutacao,
        double txCruzamento,
        double desvioPadrao,
        Funcao funcaoFitness
);

int main(){

    int
            tamanhoPopulacao = 100,
            nGeracoes = 2000,
            nParesPaisASelecionar = 15,tamTorneio = 2;
    double
            txMutacao = .05,
            txCruzamento = .9,
            desvioPadrao = 1.55,
            probMigracao = 0.003;

    std::chrono::steady_clock::time_point comeco,fim;
    std::chrono::steady_clock::duration tempo;


    Funcao funcaoFitness = Funcoes::schaffer2;
    std::vector<Populacao *> populacoes(10);

    inicializarPopulacoes(N_POPULACOES,populacoes,tamanhoPopulacao,txMutacao,txCruzamento,desvioPadrao,funcaoFitness);
    Migracao operadorMigracao(probMigracao,populacoes);

    comeco = std::chrono::steady_clock::now();

    int nPopulacoesProcessadas = 0;
    Cromossomo melhor;


    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            #pragma omp task
            operadorMigracao.iniciarMigracao(nPopulacoesProcessadas);
        }

        #pragma omp for nowait
        for (unsigned int i = 0;i < N_POPULACOES;++i){

            Populacao &p = *(populacoes[i]);

            //p.inicializacao();
            //p.calcularFitness();

            int j = 0;
            do {
                std::vector<Cromossomo> pais,filhos;
                pais = p.selecaoPais(nParesPaisASelecionar,tamTorneio);

                filhos = p.gerarFilhos(pais);

                p.selecaoSobreviventes(filhos);

                p.calcularFitness();
            } while (++j < nGeracoes && !p.verificarParada());


            #pragma omp critical
            if (melhor.getFitness() == -1 || p.getElemMaxFitness().getFitness() < melhor.getFitness())
                melhor = p.getElemMaxFitness();

            #pragma omp atomic
            nPopulacoesProcessadas += 1;

        }

    }


    fim = std::chrono::steady_clock::now();
    tempo = fim - comeco;

    double nseconds = double(tempo.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;

    std::cout << "Terminou: tempo em " << nseconds << std::endl;

    std::cout << "Melhor individuo: " << melhor << std::endl;

    return 0;
}


void inicializarPopulacoes(
        int n,
        std::vector<Populacao *> &populacoes,
        int tamanho,
        double txMutacao,
        double txCruzamento,
        double desvioPadrao,
        Funcao funcaoFitness
){
    for (unsigned int i = 0;i < n;++i) {
        populacoes[i] = new Populacao(tamanho, txMutacao, txCruzamento, desvioPadrao, funcaoFitness,true);
        populacoes[i]->inicializacao();
        populacoes[i]->calcularFitness();
    }

}

