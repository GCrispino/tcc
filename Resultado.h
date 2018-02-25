//
// Created by Crispino on 2/24/2018.
//

#ifndef TCC_RESULTADO_H
#define TCC_RESULTADO_H

/*
 *- Pior fitness
 *  - Melhor fitness
 *  - Média
 *  - Média de vezes que encontrou um fitness menor que um determinado erro
 */

struct Resultado {
    double melhorFitness;
    double piorFitness;
    double mediaFitness;

    Resultado(double melhorFitness, double mediaFitness, double piorFitness)
    :melhorFitness(melhorFitness),mediaFitness(mediaFitness),piorFitness(piorFitness)
    {}
};


#endif //TCC_RESULTADO_H
