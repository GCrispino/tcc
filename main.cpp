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
 *  - Pior fitness
 *  - Melhor fitness
 *  - Média
 *  - Média de vezes que encontrou um fitness menor que um determinado erro
 *
 *  ROADMAP
 *  - funções definidas em Algoritmos para cada tipo de AG
 *  - cada uma dessas função retorna uma instância da struct 'Resultado'
 *
 *
 */


int main(){
    //Resultado resConvencional,resParalelo,resNaoConvencional;

    /*resParalelo = *///Algoritmos::paralelo(Funcoes::rastringin,N_POPULACOES);

    Algoritmos::recombinacaoTransformacao(Funcoes::rastringin);

    return 0;
}


