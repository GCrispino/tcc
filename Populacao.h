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
        Funcao funcaoFitness;
        std::vector<Cromossomo> cromossomos;
        int iElemMaxFitness;
        int iElemMinFitness;
        Cromossomo * elemMinFitness;
        int geracaoAchouFitnessOtimo; //geração onde o fitness dentro do erro foi encontrado
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
        void calcularFitness(unsigned int geracaoAtual);
        std::vector<Cromossomo> selecaoPais(int nPaisASelecionar,int tamanhoTorneio);
        std::vector<Cromossomo> gerarFilhos(std::vector<Cromossomo> &, unsigned int geracaoAtual, unsigned int nGeracoes);
        void selecaoSobreviventes(const std::vector<Cromossomo> &);
        void setAcabou();
        bool getAcabou() const;
        int getGeracaoAchouFitnessOtimo() const;
        bool verificarParada();
        bool estaOcupada();
        const unsigned int getID();
        const Cromossomo & getElemMaxFitness();
        const Cromossomo & getElemMinFitness();
        unsigned int achaIndicePiorFitness();
        void setElemMinFitness(Cromossomo *);
        double getMediaFitness();
        int getTamanho() const;
        const Funcao getFuncaoFitness() const;
        Cromossomo getCromossomo(int i) const;
        void setCromossomo(int i,Cromossomo &);
        void setMomentoMigracao(bool);



};


#endif //TCC_POPULACAO_H
