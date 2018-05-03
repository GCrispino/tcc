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
    int nVezesAchouFitnessOtimo;
    double geracaoAchouFitnessOtimo;

    friend std::ostream &operator << (std::ostream &out, const Resultado &res){

        out << "Melhor fitness: " << res.melhorFitness << std::endl;
        out << "Pior fitness: " << res.piorFitness << std::endl;
        out << "Media do fitness: " << res.mediaFitness << std::endl;
        out << "Numero de vezes que achou o fitness otimo dentro do erro: " << res.nVezesAchouFitnessOtimo << std::endl;
        out << "(média)Geracao em que achou o fitness otimo dentro do erro: " << res.geracaoAchouFitnessOtimo;

        return out;
    }

    Resultado(
            double melhorFitness = 999,
            double mediaFitness = 999,
            double piorFitness = 999,
            int nVezesAchouFitnessOtimo = 0,
            double geracaoAchouFitnessOtimo = -1
    )
    :melhorFitness(melhorFitness),mediaFitness(mediaFitness),piorFitness(piorFitness),
     nVezesAchouFitnessOtimo(nVezesAchouFitnessOtimo),geracaoAchouFitnessOtimo(geracaoAchouFitnessOtimo)
    {}
};


#endif //TCC_RESULTADO_H
