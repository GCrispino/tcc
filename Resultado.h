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
    double melhorFitnessTotal;
    double piorFitness;
    double mediaFitness;
    double nVezesAchouFitnessOtimo;

    friend std::ostream &operator << (std::ostream &out, const Resultado &res){

        out << "Melhor fitness: " << res.melhorFitness << std::endl;
        out << "Pior fitness: " << res.piorFitness << std::endl;
        out << "Media do fitness: " << res.mediaFitness;
        out << "Numero de vezes que achou o fitness otimo dentro do erro: " << res.nVezesAchouFitnessOtimo;

        return out;
    }

    Resultado(
            double melhorFitness = 999,
            double mediaFitness = 999,
            double piorFitness = 999,
            double nVezesAchouFitnessOtimo = 0
    )
    :melhorFitness(melhorFitness),mediaFitness(mediaFitness),piorFitness(piorFitness),
     nVezesAchouFitnessOtimo(nVezesAchouFitnessOtimo)
    {}
};


#endif //TCC_RESULTADO_H
