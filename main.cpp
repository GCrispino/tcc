#include <iostream>
#include <chrono>
#include <omp.h>
#include "Populacao.h"
#include "Funcoes.hpp"

#define N_POPULACOES 10

void inicializarPopulacoes(
        std::vector<Populacao> &populacoes,
        int tamanho,
        double txMutacao,
        double txCruzamento,
        double desvioPadrao,
        Funcao funcaoFitness
);

void migracao(int &nPopulacoesProcessadas,std::vector<Populacao> &pops);

int main(){

    int
            tamanhoPopulacao = 100,
            nGeracoes = 2000,
            nParesPaisASelecionar = 15,tamTorneio = 2;
    double
            txMutacao = .05,
            txCruzamento = .9,
            desvioPadrao = 1.55;

    std::chrono::steady_clock::time_point comeco,fim;
    std::chrono::steady_clock::duration tempo;


    Funcao funcaoFitness = Funcoes::ackley;
    std::vector<Populacao> populacoes(10);

    inicializarPopulacoes(populacoes,tamanhoPopulacao,txMutacao,txCruzamento,desvioPadrao,funcaoFitness);

    comeco = std::chrono::steady_clock::now();

    int nPopulacoesProcessadas = 0;
    Cromossomo melhor;

    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            #pragma omp task
            migracao(nPopulacoesProcessadas,populacoes);
        }

        #pragma omp for nowait
        for (unsigned int i = 0;i < N_POPULACOES;++i){

            Populacao &p = populacoes[i];

            p.inicializacao();
            p.calcularFitness();

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
        std::vector<Populacao> &populacoes,
        int tamanho,
        double txMutacao,
        double txCruzamento,
        double desvioPadrao,
        Funcao funcaoFitness
){
    for (Populacao &p: populacoes)
        p = Populacao(tamanho,txMutacao,txCruzamento,desvioPadrao,funcaoFitness);

}

void migracao(int &nPopulacoesProcessadas,std::vector<Populacao> &pops){
    std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> realDis(0.0,1.0);


    while(nPopulacoesProcessadas < N_POPULACOES){
        if (realDis(gen) < 0.0)
        std::cout << "Migracao!" << std::endl;
    }
}

