//
// Created by Crispino on 2/18/2018.
//

#ifndef TCC_POPULACAOTRANSFORMACAO_H
#define TCC_POPULACAOTRANSFORMACAO_H

#include "Populacao.h"

/**
 * Possui métodos para realizar recombinação por transformação
 */

class PopulacaoTransformacao: public Populacao{
    private:
        unsigned int tamanhoPopulacaoMortos;
        std::vector<CromossomoMorto> mortos;
        unsigned int taxaInfeccao;

    public:
        PopulacaoTransformacao(
                unsigned int tamanho,unsigned int tamanhoPopulacaoMortos,
                float txMutacao, float txCruzamento, unsigned int taxaInfeccao,float desvioPadrao,
                const Funcao &funcaoFitness
        );
        void selecaoSobreviventes(const std::vector<Cromossomo> &);
        void recombinacao(unsigned int geracaoAtual, unsigned int nGeracoes);
};


#endif //TCC_POPULACAOTRANSFORMACAO_H
