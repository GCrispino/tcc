//
// Created by Crispino on 2/3/2018.
//

#include "Cromossomo.hpp"

#ifndef TCC_CROMOSSOMOMORTO_H
#define TCC_CROMOSSOMOMORTO_H


class CromossomoMorto: public Cromossomo {
    private:
        int fitMorto;

    public:
        CromossomoMorto(float txMutacao = 0.05,float desvioPadrao = 1);
        CromossomoMorto(float txMutacao,float desvioPadrao,const Funcao &funcaoFitness);
        CromossomoMorto(const Cromossomo &);
        CromossomoMorto(const CromossomoMorto &);
        CromossomoMorto& operator = (const CromossomoMorto &);
        int getFitMorto() const;
        void incFitMorto();
        void decFitMorto();
};


#endif //TCC_CROMOSSOMOMORTO_H
