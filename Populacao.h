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
        Populacao(int tamanho = 100, float txMutacao = 0.05, float txCruzamento = 0.9, float desvioPadrao = 1.55,const Funcao &funcaoFitness = Funcao());
        Populacao& operator = (const Populacao &);

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
