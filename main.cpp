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
 * - Isso é feito para cada uma das 30 execuções de cada AG
 * - Ao fim, no main, deve ser resultado um novo vector<Resultado>, com o mesmo tamanho,
 *      que irá conter as médias de cada geração
 *      a média da última geração será o resultado final a ser demonstrado
 *
 * - PROBLEMA: como fazer isso no AG paralelo, que possui 10 populações
 */


int main(){
    std::vector<Resultado> resConvencional,resParalelo,resNaoConvencional;

    resParalelo = Algoritmos::paralelo(Funcoes::rastringin,N_POPULACOES);

    //std::cout << resParalelo << std::endl;

    //Algoritmos::recombinacaoTransformacao(Funcoes::rastringin);

    return 0;
}


