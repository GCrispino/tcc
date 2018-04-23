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
        populacoes[i]->calcularFitness();
    }

}



namespace Algoritmos{

    namespace Util{
        Resultado mediaResultados(unsigned int iGeracao,const std::vector<std::vector<Resultado>> &resultsPopulacoes){
            double
                    mediaPiorFitness = 0,
                    mediaMelhorFitness = 0,
                    mediaMediaFitness = 0;

            unsigned int nExecucoes = resultsPopulacoes.size();

            for (unsigned int i = 0;i < nExecucoes;++i){
                const Resultado &resAtual = resultsPopulacoes[i][iGeracao];
                mediaPiorFitness += resAtual.piorFitness;
                mediaMediaFitness += resAtual.mediaFitness;
                mediaMelhorFitness += resAtual.melhorFitness;
            }

            mediaPiorFitness /= nExecucoes;
            mediaMediaFitness /= nExecucoes;
            mediaMelhorFitness /= nExecucoes;

            return Resultado(mediaMelhorFitness,mediaMediaFitness,mediaPiorFitness);
        }

        Resultado getResultadosGeracaoParalelo(unsigned int iGeracao,const std::vector<std::vector<Resultado>> &resultsPopulacoes){
            double
                    melhorFitnessGeracaoSubPopulacoes   = resultsPopulacoes[0][iGeracao].melhorFitness,
                    accMediaFitnessGeracaoSubPopulacoes = 0,
                    piorFitnessGeracaoSubPopulacoes     = resultsPopulacoes[0][iGeracao].piorFitness;

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
            }

            double mediaMediaFitnessGeracaoSubPopulacoes = accMediaFitnessGeracaoSubPopulacoes / nPopulacoes;


            return Resultado(
                    melhorFitnessGeracaoSubPopulacoes,
                    mediaMediaFitnessGeracaoSubPopulacoes,
                    piorFitnessGeracaoSubPopulacoes
            );

/*
            const Resultado
                    &melhorGeracaoSubPopulacoes =
                    *(std::min_element(
                            resultsPopulacoes[iGeracao].begin(),
                            resultsPopulacoes[iGeracao].end(),
                            [](Resultado r1,Resultado r2){
                                return r1.melhorFitness < r2.melhorFitness;
                            })),
                    &piorGeracaoSubPopulacoes =
                    *(std::max_element(
                            resultsPopulacoes[iGeracao].begin(),
                            resultsPopulacoes[iGeracao].end(),
                            [](Resultado r1,Resultado r2){
                                return r1.piorFitness < r2.piorFitness;
                            }));

            double
                    melhorFitnessGeracaoSubPopulacoes = melhorGeracaoSubPopulacoes.melhorFitness,
                    piorFitnessGeracaoSubPopulacoes   = piorGeracaoSubPopulacoes.piorFitness,
                    mediaFitnessSubPopulacoes         = Util::mediaResultados(iGeracao,resultsPopulacoes).mediaFitness;

            return Resultado(
                    melhorFitnessGeracaoSubPopulacoes,
                    mediaFitnessSubPopulacoes,
                    piorFitnessGeracaoSubPopulacoes
            );*/
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
        p.calcularFitness();
        do {
            std::vector<Cromossomo> pais,filhos;
            pais = p.selecaoPais(nParesPaisASelecionar,tamTorneio);

            filhos = p.gerarFilhos(pais,i,nGeracoes);

            p.selecaoSobreviventes(filhos);

            p.calcularFitness();

            results[i] = Resultado(
                    p.getElemMinFitness().getFitness(),
                    p.getElemMaxFitness().getFitness(),
                    p.getMediaFitness()
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
                tamanhoPopulacao = 10,
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
            #pragma omp single nowait
            {
                #pragma omp task
                operadorMigracao.iniciarMigracao(nPopulacoesProcessadas);
            }

            #pragma omp for nowait
            for (unsigned int i = 0;i < N_POPULACOES;++i){

                Populacao &p = *(populacoes[i]);

                #pragma omp critical
                std::cout << "Iniciou populacao " << p.getID() << std::endl;


                int j = 0;
                do {

                    std::vector<Cromossomo> pais,filhos;
                    pais = p.selecaoPais(nParesPaisASelecionar,tamTorneio);

                    filhos = p.gerarFilhos(pais,i,nGeracoes);

                    p.selecaoSobreviventes(filhos);

                    p.calcularFitness();

                    /*if (j % 100 == 0) {
                        std::cout << "Populacao: " << i << std::endl;
                        std::cout << "Geracao: " << j << '\t';
                        std::cout << "Melhor fitness: " << p.getElemMinFitness().getFitness() << '\t';
                        std::cout << "Media do fitness: " << p.getMediaFitness();
                        std::cout << std::endl;
                    }*/

                    resultsPopulacoes[i].push_back(Resultado(
                            p.getElemMinFitness().getFitness(),
                            p.getElemMaxFitness().getFitness(),
                            p.getMediaFitness()
                    ));
                } while (++j < nGeracoes);
                p.setAcabou();

                #pragma omp critical
                if (melhor.getFitness() == -1 || p.getElemMinFitness().getFitness() < melhor.getFitness())
                    melhor = p.getElemMinFitness();

                #pragma omp critical
                if (pior.getFitness() == -1 || p.getElemMaxFitness().getFitness() > pior.getFitness())
                    pior = p.getElemMaxFitness();


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

        std::vector<Cromossomo>melhores(N_POPULACOES);

        std::transform(
                populacoes.begin(),
                populacoes.end(),
                melhores.begin(),
                [](Populacao *p){
                    return p->getElemMaxFitness();
                }
        );

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
        p.calcularFitness();

        do {
            std::vector<Cromossomo> pais,filhos;
            pais = p.selecaoPais(nParesPaisASelecionar,tamTorneio);

            filhos = p.gerarFilhos(pais,i,nGeracoes);

            p.selecaoSobreviventes(filhos);

            p.calcularFitness();

            p.recombinacao(i,nGeracoes);
            if (i % 100 == 0) {
                std::cout << "Geracao: " << i << '\t';
                std::cout << "Melhor fitness: " << p.getElemMinFitness().getFitness() << '\t';
                std::cout << "Pior fitness: " << p.getElemMaxFitness().getFitness() << '\t';
                std::cout << "Media do fitness: " << p.getMediaFitness();
                std::cout << std::endl;
            }


            results[i] = Resultado(
                    p.getElemMinFitness().getFitness(),
                    p.getElemMaxFitness().getFitness(),
                    p.getMediaFitness()
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