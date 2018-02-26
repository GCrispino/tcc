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
 * - executar 30 vezes cada algoritmo e pegar o resultado
 * - calcular a média desses resultados
 */


int main(){
    std::vector<Resultado> resConvencional,resParalelo,resNaoConvencional;

    resParalelo = Algoritmos::paralelo(Funcoes::rastringin,N_POPULACOES);

    //std::cout << resParalelo << std::endl;

    //Algoritmos::recombinacaoTransformacao(Funcoes::rastringin);

    return 0;
}


