//
// Created by Crispino on 1/12/2018.
//

#ifndef TCC_POPULACAO_H
#define TCC_POPULACAO_H


#include "Cromossomo.hpp"
#include "CromossomoMorto.hpp"

class Populacao {
    protected:
        unsigned int id;
        unsigned int tamanho;
        float txMutacao;
        float txCruzamento;
        float desvioPadrao;
        double mediaFitness;
        Funcao funcaoFitness;
        std::vector<Cromossomo> cromossomos;
        int iElemMaxFitness;
        int iElemMinFitness;
        Cromossomo * elemMaxFitness;
        bool paralelo;
        bool momentoMigracao;//define se migração está acontecendo no momento
        bool acabou;
        bool ocupada;

        static std::mt19937 *gen;
        static unsigned int count;

        const int N_THREADS = 3;

    public:
        Populacao(
                unsigned int tamanho,float txMutacao, float txCruzamento,
                float desvioPadrao, const Funcao &funcaoFitness,
                bool paralelo = false
        );

        void inicializacao();
        void calcularFitness();
        std::vector<Cromossomo> selecaoPais(int nPaisASelecionar,int tamanhoTorneio);
        std::vector<Cromossomo> gerarFilhos(std::vector<Cromossomo> &);
        void selecaoSobreviventes(const std::vector<Cromossomo> &);
        void setAcabou();
        bool verificarParada();
        bool estaOcupada();
        const unsigned int getID();
        const Cromossomo & getElemMaxFitness();
        const Cromossomo & getElemMinFitness();
        unsigned int achaIndicePiorFitness();
        void setElemMaxFitness(Cromossomo *);
        double getMediaFitness();
        int getTamanho() const;
        Cromossomo getCromossomo(int i) const;
        void setCromossomo(int i,Cromossomo &);
        void setMomentoMigracao(bool);



};


#endif //TCC_POPULACAO_H
