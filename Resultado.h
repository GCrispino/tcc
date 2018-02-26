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

/**
 * Resultado da execução de cada geração de um AG
 */
struct Resultado {
    double melhorFitness;
    double piorFitness;
    double mediaFitness;

    friend std::ostream &operator << (std::ostream &out, const Resultado &res){

        out << "Melhor fitness: " << res.melhorFitness << std::endl;
        out << "Pior fitness: " << res.piorFitness << std::endl;
        out << "Média do fitness: " << res.mediaFitness;

        return out;
    }

    Resultado(double melhorFitness = 999, double mediaFitness = 999, double piorFitness = 999)
    :melhorFitness(melhorFitness),mediaFitness(mediaFitness),piorFitness(piorFitness)
    {}
};


#endif //TCC_RESULTADO_H
