//
// Created by Crispino on 1/12/2018.
//

#ifndef TCC_POPULACAO_H
#define TCC_POPULACAO_H


#include "Cromossomo.hpp"

class Populacao {
    private:
        int tamanho;
        float txMutacao;
        float txCruzamento;
        float desvioPadrao;
        double mediaFitness;
        Funcao funcaoFitness;
        std::vector<Cromossomo> cromossomos;
        int iElemMaxFitness;
        int iElemMinFitness;
        Cromossomo * elemMaxFitness;
        bool acabou;

        static std::default_random_engine *gen;

        const int N_THREADS = 3;

    public:
        Populacao(int tamanho, float txMutacao, float txCruzamento, float desvioPadrao,const Funcao &funcaoFitness);

        void inicializacao();
        void calcularFitness();
        std::vector<Cromossomo> selecaoPais(int nPaisASelecionar,int tamanhoTorneio);
        std::vector<Cromossomo> gerarFilhos(std::vector<Cromossomo> &);
        void selecaoSobreviventes(const std::vector<Cromossomo> &);
        bool verificarParada();
        void setAcabou();
        const Cromossomo & getElemMaxFitness();
        const Cromossomo & getElemMinFitness();
        unsigned int achaIndicePiorFitness();
        void setElemMaxFitness(Cromossomo *);
        double getMediaFitness();



};


#endif //TCC_POPULACAO_H
