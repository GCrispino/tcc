#include <iostream>
#include <chrono>
#include <algorithm>
#include <omp.h>
#include "Algoritmos.hpp"
#include "Funcoes.hpp"


#define N_POPULACOES 10


/**
 *
 * O QUE AVALIAR NO DESEMPENHO DO ALGORITMO
 *
 *
 *  - Pior fitness - feito
 *  - Melhor fitness - feito
 *  - Média fitness - Média da geração e/ou média da população no algoritmo inteiro?
 *  - Média de vezes que encontrou um fitness menor que um determinado erro
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
std::vector<std::vector<Resultado>> executarAGSequencialNVezes(unsigned int n,std::vector<Resultado> (*ag)(const Funcao &));
std::vector<std::vector<Resultado>> executarAGParaleloNVezes(unsigned int n,std::vector<Resultado> (*)(const Funcao &,const int));

int main(){
    Resultado resFinalConvencional,resFinalParalelo,resFinalNaoConvencional;
    std::vector<Resultado> mediaResultsConvencional,mediaResultsParalelo,mediaResultsNaoConvencional;
    std::vector<std::vector<Resultado>> resultsConvencional,resultsParalelo,resultsNaoConvencional;

    unsigned int nExecucoes = 30,nGeracoes = 1000;

    //resParalelo = Algoritmos::paralelo(Funcoes::rastringin,N_POPULACOES);
    //resultsParalelo = executarAGParaleloNVezes(nExecucoes,Algoritmos::paralelo);
    std::cout << "Executando AG sequencial convencional..." << std::endl;
    resultsConvencional = executarAGSequencialNVezes(nExecucoes,Algoritmos::convencional);
    std::cout << "Executando AG sequencial não-convencional..." << std::endl;
    resultsNaoConvencional = executarAGSequencialNVezes(nExecucoes,Algoritmos::recombinacaoTransformacao);

    std::cout << "Calculando resultados..." << std::endl;

    //mediaResultsParalelo = getMediaNExecucoes(nGeracoes,resultsParalelo);
    mediaResultsConvencional = getMediaNExecucoes(nGeracoes,resultsConvencional);
    mediaResultsNaoConvencional = getMediaNExecucoes(nGeracoes,resultsNaoConvencional);

    //resFinalParalelo = mediaResultsParalelo[mediaResultsParalelo.size() - 1];
    resFinalConvencional= mediaResultsConvencional[mediaResultsConvencional.size() - 1];
    resFinalNaoConvencional= mediaResultsNaoConvencional[mediaResultsNaoConvencional.size() - 1];


    std::cout << "Resultado Convencional: " << std::endl;
    std::cout << resFinalConvencional << std::endl << std::endl;
    //std::cout << "Resultado Paralelo: " << std::endl;
    //std::cout << resFinalParalelo << std::endl << std::endl;
    std::cout << "Resultado Não-Convencional: " << std::endl;
    std::cout << resFinalNaoConvencional << std::endl << std::endl;


    return 0;
}

std::vector<Resultado> getMediaNExecucoes(unsigned int nGeracoes,const std::vector<std::vector<Resultado>> &resultsExecucoes){

    unsigned int nExecucoes = resultsExecucoes.size();

    std::vector<Resultado> resultsMediasExecucoes(nGeracoes);

    for (unsigned int i = 0;i < nGeracoes;++i)
        resultsMediasExecucoes[i] = Algoritmos::Util::mediaResultados(i,resultsExecucoes);


    return resultsMediasExecucoes;
}

std::vector<std::vector<Resultado>> executarAGSequencialNVezes(unsigned int n,std::vector<Resultado> (*ag)(const Funcao &)){
    std::vector<std::vector<Resultado>> results(n);
    for (int i = 0;i < n;++i)
        results[i] = ag(Funcoes::rosenbrock);

    return results;
}

std::vector<std::vector<Resultado>> executarAGParaleloNVezes(unsigned int n,std::vector<Resultado> (*ag)(const Funcao &,const int)){
    std::vector<std::vector<Resultado>> results(n);
    for (int i = 0;i < n;++i)
        results[i] = ag(Funcoes::rosenbrock,N_POPULACOES);

    return results;
}
