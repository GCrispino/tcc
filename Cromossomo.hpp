#ifndef TCC_CROMOSSOMO_H
#define TCC_CROMOSSOMO_H

#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include "Funcao.hpp"

class Cromossomo{

    friend std::ostream &operator << (std::ostream &, const Cromossomo &);

    private:
        const static int N_GENES = 2;
        float txMutacao;
        float desvioPadrao;
        std::vector<double> genotipo;
        double fitness;
        Funcao funcaoFitness;
        static std::mt19937 *gen;
        std::uniform_real_distribution<float> realDis;
        std::uniform_real_distribution<double> disIntervaloValoresFuncao;
        std::uniform_int_distribution<int> intDis;
        std::normal_distribution<double> gaussianDis;

    public:
        Cromossomo(float txMutacao = 0.05,float desvioPadrao = 1);
        Cromossomo(float txMutacao,float desvioPadrao,const Funcao &funcaoFitness);
        Cromossomo(const Cromossomo &);
        Cromossomo& operator = (const Cromossomo &);


    void inicializar();

        void calcularFitness();

        void mutacao(unsigned int geracaoAtual, unsigned int nGeracoes,bool calculaProbabilidadeCadaFilho = false);

        //realiza a opera??o de muta??o probabilisticamente em um array de cromossomos
        void mutaFilhos(std::vector<Cromossomo> &filhos, unsigned int geracaoAtual, unsigned int nGeracoes);

        //gera dois filhos
        std::vector<Cromossomo> crossover(const Cromossomo &outroCromossomo,unsigned int geracaoAtual, unsigned int nGeracoes);

        //gera um filho
        Cromossomo crossoverSimples(const Cromossomo &outroCromossomo, unsigned int geracaoAtual, unsigned int nGeracoes);

        double getFitness() const;

        void setFuncao(const Funcao &);

};

#endif //TCC_CROMOSSOMO_H
