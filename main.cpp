#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <omp.h>
#include <cstring>
#include "Algoritmos.hpp"
#include "Funcoes.hpp"


#define N_POPULACOES 10
#define N_THREADS 4


/**
 * PROBLEMA:
 *  - Ponteiros de geradores de números aleatórios precisam ser liberados usando delete
 *      - mas eles são estáticos
 *      - soluções:
 *          - passá-los como parâmetro no construtor
 *          - não definí-los como estáticos
 *              - talvez isso possa ter maus efeitos estatísticos
 */


/**
 *
 * O QUE FAZER NO MAIN:
 * - Cada função retorna um std::vector<Resultado>
 * - Esse vector irá conter os resultados para cada geração do algoritmo
 * - Cada tipo de AG é executado então, N vezes, no main
 * - Ao final disso, temos um std::vector<std::vector<Resultado>>(n),
 *      com os resultados de cada geração para cada uma das n execuções
 * - Um std::vector<Resultado> do mesmo tamanho do retorno das funções vai ser originado,
 *      da média das n execuções, para cada geração.
 * - O resultado final é o último elemento desse vector final
 *
 * - PROBLEMA: como fazer isso no AG paralelo, que possui 10 populações
 * - SOLUÇÃO: para cada população, tirar a média dos resultados de cada geração i para as 30 execuções
 */

std::vector<Resultado> getMediaNExecucoes(unsigned int nGeracoes, const std::vector<std::vector<Resultado>> &resultsExecucoes);
Resultado getResultadoAbsolutoExecucoes(const std::vector<std::vector<Resultado>> &resultsExecucoes);
Resultado getResultadoAbsoluto(const std::vector<Resultado> &resultsExecucao);
std::vector<std::vector<Resultado>> executarAGSequencialNVezes(unsigned int n,std::vector<Resultado> (*ag)(const Funcao &),const Funcao &funcao);
std::vector<std::vector<Resultado>> executarAGParaleloNVezes(unsigned int n,std::vector<Resultado> (*)(const Funcao &,const int),const Funcao &funcao);
std::string criarPastaArquivos();
void escreveResultados(std::ofstream &nomeArquivo, const std::string &nomeFuncao,const Resultado &resFinal,const Resultado &resAbsoluto);

int main(){

    omp_set_num_threads(N_THREADS);


    std::string nomePastaArquivos = criarPastaArquivos();
    std::ofstream
            arqConvencional(nomePastaArquivos + "/convencional.txt"),
            arqParalelo(nomePastaArquivos + "/paralelo.txt"),
            arqNaoConvencional(nomePastaArquivos + "/nao-convencional.txt"),
            arqNaoConvencionalParalelo(nomePastaArquivos + "/nao-convencional-paralelo.txt");

    unsigned int nExecucoes = 30,nGeracoes = 1000;

    for (Funcao &funcao: Funcoes::funcoes) {
        Resultado
                resFinalConvencional, resAbsolutoConvencional,
                resFinalParalelo, resAbsolutoParalelo,
                resFinalNaoConvencional, resAbsolutoNaoConvencional,
                resFinalNaoConvencionalParalelo, resAbsolutoNaoConvencionalParalelo;
        std::vector<Resultado> mediaResultsConvencional,mediaResultsParalelo,mediaResultsNaoConvencional,mediaResultsNaoConvencionalParalelo;
        std::vector<std::vector<Resultado>> resultsConvencional,resultsParalelo,resultsNaoConvencional,resultsNaoConvencionalParalelo;

        std::cout << "Executando AG sequencial convencional..." << std::endl;
        resultsConvencional = executarAGSequencialNVezes(nExecucoes, Algoritmos::convencional, funcao);
        std::cout << "Executando AG sequencial não-convencional..." << std::endl;
        resultsNaoConvencional = executarAGSequencialNVezes(nExecucoes, Algoritmos::recombinacaoTransformacao, funcao);
        std::cout << "Executando AG paralelo convencional..." << std::endl;
        resultsParalelo = executarAGParaleloNVezes(nExecucoes, Algoritmos::paralelo, funcao);
        std::cout << "Executando AG paralelo nao-convencional..." << std::endl;
        resultsNaoConvencionalParalelo = executarAGParaleloNVezes(nExecucoes,
                                                                  Algoritmos::recombinacaoTransformacaoParalelo,
                                                                  funcao);

        std::cout << "Calculando resultados..." << std::endl;

        mediaResultsParalelo = getMediaNExecucoes(nGeracoes, resultsParalelo);
        mediaResultsConvencional = getMediaNExecucoes(nGeracoes, resultsConvencional);
        mediaResultsNaoConvencional = getMediaNExecucoes(nGeracoes, resultsNaoConvencional);
        mediaResultsNaoConvencionalParalelo = getMediaNExecucoes(nGeracoes, resultsNaoConvencionalParalelo);

        resAbsolutoParalelo = getResultadoAbsolutoExecucoes(resultsParalelo);
        resFinalParalelo = mediaResultsParalelo[mediaResultsParalelo.size() - 1];
        resAbsolutoConvencional = getResultadoAbsolutoExecucoes(resultsConvencional);
        resFinalConvencional = mediaResultsConvencional[mediaResultsConvencional.size() - 1];
        resAbsolutoNaoConvencional = getResultadoAbsolutoExecucoes(resultsNaoConvencional);
        resFinalNaoConvencional = mediaResultsNaoConvencional[mediaResultsNaoConvencional.size() - 1];
        resAbsolutoNaoConvencionalParalelo = getResultadoAbsolutoExecucoes(resultsNaoConvencionalParalelo);
        resFinalNaoConvencionalParalelo = mediaResultsNaoConvencionalParalelo[
                mediaResultsNaoConvencionalParalelo.size() - 1];

        /*std::cout << "Resultado Convencional: " << std::endl;
        std::cout << resFinalConvencional << std::endl << std::endl;
        std::cout << "Resultado Absoluto Convencional: " << std::endl;
        std::cout << resAbsolutoConvencional << std::endl << std::endl;
        std::cout << "Resultado Não-Convencional: " << std::endl;
        std::cout << resFinalNaoConvencional << std::endl << std::endl;
        std::cout << "Resultado Absoluto Nao-Convencional: " << std::endl;
        std::cout << resAbsolutoNaoConvencional << std::endl << std::endl;
        std::cout << "Resultado Paralelo: " << std::endl;
        std::cout << resFinalParalelo << std::endl << std::endl;
        std::cout << "Resultado Absoluto Paralelo: " << std::endl;
        std::cout << resAbsolutoParalelo << std::endl << std::endl;
        std::cout << "Resultado Paralelo nao-convencional: " << std::endl;
        std::cout << resFinalNaoConvencionalParalelo << std::endl << std::endl;
        std::cout << "Resultado Absoluto Paralelo nao-convencional: " << std::endl;
        std::cout << resAbsolutoNaoConvencionalParalelo << std::endl << std::endl;*/

        std::cout << "Escrevendo resultados na pasta " << nomePastaArquivos;
        std::cout << " para a funcao " << funcao.getNome() << "..." << std::endl;
        escreveResultados(arqConvencional, funcao.getNome(), resFinalConvencional, resAbsolutoConvencional);
        escreveResultados(arqParalelo, funcao.getNome(), resFinalParalelo, resAbsolutoParalelo);
        escreveResultados(arqNaoConvencional, funcao.getNome(), resFinalNaoConvencional, resAbsolutoNaoConvencional);
        escreveResultados(arqNaoConvencionalParalelo, funcao.getNome(), resFinalNaoConvencionalParalelo,
                          resAbsolutoNaoConvencionalParalelo);
        std::cout << "Resultados escritos! " << std::endl;
    }

    return 0;
}

//Retorna resultados absolutos a partir de um vector de resultados
Resultado getResultadoAbsoluto(const std::vector<Resultado> &resultsExecucao){
    return std::accumulate(
            resultsExecucao.begin(),
            resultsExecucao.end(),
            Resultado(),
            [](Resultado acc, Resultado novo){
                return Resultado(
                    acc.melhorFitness < novo.melhorFitness ? acc.melhorFitness : novo.melhorFitness,
                    0,
                    acc.piorFitness > novo.piorFitness ? acc.piorFitness : novo.piorFitness,
                    0,
                    novo.geracaoAchouFitnessOtimo
                );
            }
    );
}

Resultado getResultadoAbsolutoExecucoes(const std::vector<std::vector<Resultado>> &resultsExecucoes){
    std::vector<Resultado> resultsAbsolutosExecucoes(resultsExecucoes.size());

    //Encher o vector resultsAbsolutosExecucoes com os novos calculos
    std::transform(
            resultsExecucoes.begin(),
            resultsExecucoes.end(),
            resultsAbsolutosExecucoes.begin(),
            getResultadoAbsoluto
    );


    return std::accumulate(
            resultsAbsolutosExecucoes.begin(),
            resultsAbsolutosExecucoes.end(),
            Resultado(),
            [](Resultado acc, Resultado novo){
                double novoGeracaoAchouFitnessOtimo =
                        novo.geracaoAchouFitnessOtimo == -1
                            ? acc.geracaoAchouFitnessOtimo
                            : (acc.geracaoAchouFitnessOtimo + novo.geracaoAchouFitnessOtimo) / 2;

                return Resultado(
                        acc.melhorFitness < novo.melhorFitness ? acc.melhorFitness : novo.melhorFitness,
                        0,
                        acc.piorFitness > novo.piorFitness ? acc.piorFitness : novo.piorFitness,
                        0,
                        novoGeracaoAchouFitnessOtimo
                );
            }
    );
}

std::vector<Resultado> getMediaNExecucoes(unsigned int nGeracoes,const std::vector<std::vector<Resultado>> &resultsExecucoes){

    unsigned int nExecucoes = resultsExecucoes.size();

    std::vector<Resultado> resultsMediasExecucoes(nGeracoes);

    for (unsigned int i = 0;i < nGeracoes;++i)
        resultsMediasExecucoes[i] = Algoritmos::Util::mediaResultados(i,resultsExecucoes);



    return resultsMediasExecucoes;
}

std::vector<std::vector<Resultado>> executarAGSequencialNVezes(unsigned int n,std::vector<Resultado> (*ag)(const Funcao &),const Funcao &funcao){
    std::vector<std::vector<Resultado>> results(n);
    for (int i = 0;i < n;++i)
        results[i] = ag(funcao);

    return results;
}

std::vector<std::vector<Resultado>> executarAGParaleloNVezes(unsigned int n,std::vector<Resultado> (*ag)(const Funcao &,const int),const Funcao &funcao){
    std::vector<std::vector<Resultado>> results(n);
    for (int i = 0;i < n;++i)
        results[i] = ag(funcao, N_POPULACOES);

    return results;
}

std::string criarPastaArquivos(){
    time_t now = time(0);
    std::string nomePasta = std::to_string(now);
    const char *c_nomePasta = nomePasta.c_str();
    char comando[255];
    strcpy(comando,"mkdir ");
    strcat(comando,c_nomePasta);


    system(comando);

    return nomePasta;
}

void escreveResultados(std::ofstream &arquivo, const std::string &nomeFuncao,const Resultado &resFinal,const Resultado &resAbsoluto){
    arquivo << "Resultado final para a funcao " << nomeFuncao << std::endl;
    arquivo << resFinal << std::endl << std::endl;

    arquivo << "Resultado absoluto para a funcao " << nomeFuncao << std::endl;
    arquivo << resAbsoluto << std::endl << std::endl;
}
