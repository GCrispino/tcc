//
// Created by Crispino on 2/24/2018.
//
#include <iostream>
#include <chrono>
#include <algorithm>
#include "Resultado.h"
#include "Funcao.hpp"
#include "Populacao.h"
#include "PopulacaoTransformacao.h"
#include "Migracao.h"

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
        populacoes[i]->calcularFitness(0);
    }

}



namespace Algoritmos{

    namespace Util{
        Resultado mediaResultados(unsigned int iGeracao,const std::vector<std::vector<Resultado>> &resultsPopulacoes){
            double
                    mediaPiorFitness = 0,
                    mediaMelhorFitness = 0,
                    mediaMediaFitness = 0;
            int
                    nVezesAchouFitnessOtimo = 0;

            unsigned int nExecucoes = resultsPopulacoes.size();

            for (unsigned int i = 0;i < nExecucoes;++i){
                const Resultado &resAtual = resultsPopulacoes[i][iGeracao];
                mediaPiorFitness              += resAtual.piorFitness;
                mediaMediaFitness             += resAtual.mediaFitness;
                mediaMelhorFitness            += resAtual.melhorFitness;
                nVezesAchouFitnessOtimo       += resAtual.nVezesAchouFitnessOtimo;
            }

            mediaPiorFitness /= nExecucoes;
            mediaMediaFitness /= nExecucoes;
            mediaMelhorFitness /= nExecucoes;

            return Resultado(mediaMelhorFitness,mediaMediaFitness,mediaPiorFitness,nVezesAchouFitnessOtimo);
        }

        Resultado getResultadosGeracaoParalelo(unsigned int iGeracao,const std::vector<std::vector<Resultado>> &resultsPopulacoes){
            double
                    melhorFitnessGeracaoSubPopulacoes          = resultsPopulacoes[0][iGeracao].melhorFitness,
                    accMediaFitnessGeracaoSubPopulacoes        = 0,
                    piorFitnessGeracaoSubPopulacoes            = resultsPopulacoes[0][iGeracao].piorFitness,
                    achouFitnessOtimoSubPopulacoes             = resultsPopulacoes[0][iGeracao].nVezesAchouFitnessOtimo,
                    geracaoAchouFitnessOtimoSubPopulacoes      = resultsPopulacoes[0][iGeracao].geracaoAchouFitnessOtimo;

            unsigned int nPopulacoes = resultsPopulacoes.size();

            /**
             * Acha o resultado que possui o melhor valor, o que possui o pior valor de fitness,
             *  e calcula a soma das médias, tudo para cada subpopulação
             */
            for (unsigned int i = 0;i < nPopulacoes;++i){
                const Resultado &resAtual = resultsPopulacoes[i][iGeracao];
                melhorFitnessGeracaoSubPopulacoes =
                        resAtual.melhorFitness < melhorFitnessGeracaoSubPopulacoes ? resAtual.melhorFitness : melhorFitnessGeracaoSubPopulacoes;
                piorFitnessGeracaoSubPopulacoes =
                        resAtual.piorFitness > piorFitnessGeracaoSubPopulacoes ? resAtual.piorFitness : piorFitnessGeracaoSubPopulacoes;
                accMediaFitnessGeracaoSubPopulacoes += resAtual.piorFitness;

                if (achouFitnessOtimoSubPopulacoes != 1)
                    achouFitnessOtimoSubPopulacoes = resAtual.nVezesAchouFitnessOtimo;

                if (resAtual.geracaoAchouFitnessOtimo != -1)
                    geracaoAchouFitnessOtimoSubPopulacoes = resAtual.geracaoAchouFitnessOtimo;
            }

            double mediaMediaFitnessGeracaoSubPopulacoes = accMediaFitnessGeracaoSubPopulacoes / nPopulacoes;

            return Resultado(
                    melhorFitnessGeracaoSubPopulacoes,
                    mediaMediaFitnessGeracaoSubPopulacoes,
                    piorFitnessGeracaoSubPopulacoes,
                    achouFitnessOtimoSubPopulacoes,
                    geracaoAchouFitnessOtimoSubPopulacoes
            );
        }

    }

    std::vector<Resultado> convencional(const Funcao &funcaoFitness){

        Populacao p(100,0.05,0.9,1.55,funcaoFitness);

        int
                i = 0,
                nGeracoes = 1000,
                nParesPaisASelecionar = 10,tamTorneio = 2;

        std::vector<Resultado> results(nGeracoes);

        std::chrono::steady_clock::time_point comeco,fim;
        std::chrono::steady_clock::duration tempo;

        comeco = std::chrono::steady_clock::now();

        p.inicializacao();
        p.calcularFitness(0);
        do {
            std::vector<Cromossomo> pais,filhos;
            pais = p.selecaoPais(nParesPaisASelecionar,tamTorneio);

            filhos = p.gerarFilhos(pais,i,nGeracoes);

            p.selecaoSobreviventes(filhos);

            p.calcularFitness(i);

            bool achouFitnessOtimo =
                    p.getElemMinFitness().getFitness() < p.getFuncaoFitness().getMinimoGlobal() + pow(10,-3);

            results[i] = Resultado(
                    p.getElemMinFitness().getFitness(),
                    p.getElemMaxFitness().getFitness(),
                    p.getMediaFitness(),
                    achouFitnessOtimo,
                    p.getGeracaoAchouFitnessOtimo()
            );


            if (i % 100 == 0) {
                std::cout << "Geracao: " << i << '\t';
                std::cout << "Melhor fitness: " << p.getElemMinFitness().getFitness() << '\t';
                std::cout << "Pior fitness: " << p.getElemMaxFitness().getFitness() << '\t';
                std::cout << "Media do fitness: " << p.getMediaFitness();
                std::cout << std::endl;
            }
        } while (++i < nGeracoes);


        const Cromossomo &melhor = p.getElemMinFitness();

        fim = std::chrono::steady_clock::now();
        tempo = fim - comeco;

        double nseconds = double(tempo.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;

        /*
        std::cout << "Terminou: tempo em " << nseconds << std::endl;
        std::cout << "Numero de geracoes: " << i << std::endl;
        std::cout << "Melhor individuo: " << melhor << std::endl;
        std::cout << "Media do fitness da populacao: " << p.getMediaFitness();
*/

        return results;
    }


    std::vector<Resultado> paralelo(const Funcao &funcaoFitness,const int N_POPULACOES){

        int
                tamanhoPopulacao = 80,
                nGeracoes = 1000,
                nParesPaisASelecionar = 10,tamTorneio = 2;
        double
                txMutacao = .05,
                txCruzamento = .9,
                desvioPadrao = 1.55,
                probMigracao = 0.003;

        std::vector<std::vector<Resultado>> resultsPopulacoes(N_POPULACOES);

        std::chrono::steady_clock::time_point comeco,fim;
        std::chrono::steady_clock::duration tempo;

        std::vector<Populacao *> populacoes(N_POPULACOES);

        inicializarPopulacoes(N_POPULACOES,populacoes,tamanhoPopulacao,txMutacao,txCruzamento,desvioPadrao,funcaoFitness);
        Migracao operadorMigracao(probMigracao,populacoes);

        comeco = std::chrono::steady_clock::now();

        int nPopulacoesProcessadas = 0;
        Cromossomo melhor,pior;


        #pragma omp parallel
        {
            #pragma omp for nowait
            for (unsigned int i = 0;i < N_POPULACOES;++i){

                Populacao &p = *(populacoes[i]);

                #pragma omp critical
                std::cout << "Iniciou populacao " << p.getID() << std::endl;


                int j = 0;
                do {
                    bool jaAchou = false;
                    std::vector<Cromossomo> pais,filhos;
                    pais = p.selecaoPais(nParesPaisASelecionar,tamTorneio);

                    filhos = p.gerarFilhos(pais,j,nGeracoes);

                    p.selecaoSobreviventes(filhos);

                    p.calcularFitness(j);

                    /*if (j % 100 == 0) {
                        std::cout << "Populacao: " << i << std::endl;
                        std::cout << "Geracao: " << j << '\t';
                        std::cout << "Melhor fitness: " << p.getElemMinFitness().getFitness() << '\t';
                        std::cout << "Media do fitness: " << p.getMediaFitness();
                        std::cout << std::endl;
                    }*/
                    bool achouFitnessOtimo =
                            p.getElemMinFitness().getFitness() < p.getFuncaoFitness().getMinimoGlobal() + pow(10,-3);


                    resultsPopulacoes[i].push_back(Resultado(
                            p.getElemMinFitness().getFitness(),
                            p.getElemMaxFitness().getFitness(),
                            p.getMediaFitness(),
                            achouFitnessOtimo,
                            p.getGeracaoAchouFitnessOtimo()
                    ));


                /*    if (omp_get_thread_num() == 0)
                        operadorMigracao.realizarMigracao(nPopulacoesProcessadas);*/


                    #pragma omp critical
                    {
                        if (p.getID() % N_POPULACOES == 1)
                            operadorMigracao.realizarMigracao(nPopulacoesProcessadas);
                    }

                    /*#pragma omp critical
                    {
                        operadorMigracao.realizarMigracao(nPopulacoesProcessadas);
                    }*/
                } while (++j < nGeracoes);

                p.setAcabou();


                #pragma omp atomic
                nPopulacoesProcessadas += 1;

                #pragma omp critical
                {
                    std::cout << "Terminou populacao " << p.getID() << std::endl;
                    //std::cout << "Melhor elemento: " << p.getElemMinFitness() << std::endl;
                    std::cout << "";
                }

            }

        }

        fim = std::chrono::steady_clock::now();
        tempo = fim - comeco;

        double nseconds = double(tempo.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;

        std::cout << "Terminou: tempo em " << nseconds << std::endl;

        //std::cout << "Melhor individuo: " << melhor << std::endl;

        std::cout << "===============================================================";
        std::cout << "===============================================================" << std::endl;

        std::vector<Resultado> resultsFinais(nGeracoes);
        /**
         * transformar resultsPopulacoes em resultsFinais, pegando o melhor dos melhores
         *  da geração para cada subpopulação, o pior dos piores, e a média das médias,
         *  também dos resultados dessa geração para cada subpopulação
         */
        for(unsigned int i = 0;i < nGeracoes;++i) {
            resultsFinais[i] = Util::getResultadosGeracaoParalelo(i, resultsPopulacoes);
        }


        return resultsFinais;
    }


    std::vector<Resultado> recombinacaoTransformacao(const Funcao &funcaoFitness){
        PopulacaoTransformacao p(100,50,0.05,0.9,30,1.55,funcaoFitness);

        int
                i = 0,
                nGeracoes = 1000,
                nParesPaisASelecionar = 10,tamTorneio = 2;

        std::vector<Resultado> results(nGeracoes);

        std::chrono::steady_clock::time_point comeco,fim;
        std::chrono::steady_clock::duration tempo;

        comeco = std::chrono::steady_clock::now();

        p.inicializacao();
        p.calcularFitness(0);

        do {
            std::vector<Cromossomo> pais,filhos;
            pais = p.selecaoPais(nParesPaisASelecionar,tamTorneio);

            filhos = p.gerarFilhos(pais,i,nGeracoes);

            p.selecaoSobreviventes(filhos);

            p.calcularFitness(i);

            p.recombinacao(i,nGeracoes);
            if (i % 100 == 0) {
                std::cout << "Geracao: " << i << '\t';
                std::cout << "Melhor fitness: " << p.getElemMinFitness().getFitness() << '\t';
                std::cout << "Pior fitness: " << p.getElemMaxFitness().getFitness() << '\t';
                std::cout << "Media do fitness: " << p.getMediaFitness();
                std::cout << std::endl;
            }

            bool achouFitnessOtimo =
                    p.getElemMinFitness().getFitness() < p.getFuncaoFitness().getMinimoGlobal() + pow(10,-3);

            results[i] = Resultado(
                    p.getElemMinFitness().getFitness(),
                    p.getElemMaxFitness().getFitness(),
                    p.getMediaFitness(),
                    achouFitnessOtimo,
                    p.getGeracaoAchouFitnessOtimo()
            );
        } while (++i < nGeracoes);

        const Cromossomo &melhor = p.getElemMinFitness();

        fim = std::chrono::steady_clock::now();
        tempo = fim - comeco;

        double nseconds = double(tempo.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;


        /*std::cout << "Terminou: tempo em " << nseconds << std::endl;
        std::cout << "Numero de geracoes: " << i << std::endl;
        std::cout << "Melhor individuo: " << melhor << std::endl;
        std::cout << "Media do fitness da populacao: " << p.getMediaFitness() << std::endl;
*/
        return results;
    }


}