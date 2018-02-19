//
// Created by Crispino on 2/3/2018.
//

#include "CromossomoMorto.hpp"

CromossomoMorto::CromossomoMorto(float txMutacao,float desvioPadrao)
:Cromossomo(txMutacao,desvioPadrao){
    this->fitMorto = 0;
}

CromossomoMorto::CromossomoMorto(float txMutacao,float desvioPadrao,const Funcao &funcaoFitness)
:Cromossomo(txMutacao,desvioPadrao,funcaoFitness){
    this->fitMorto = 0;
}

CromossomoMorto::CromossomoMorto(const Cromossomo &c) :Cromossomo(c){
    this->fitMorto = 0;
}

CromossomoMorto::CromossomoMorto(const CromossomoMorto &c)
:Cromossomo(c){
    this->fitMorto = c.fitMorto;
}

CromossomoMorto& CromossomoMorto::operator = (const CromossomoMorto &c) {
    if (this == &c)
        return *this;

    Cromossomo::operator=(c);

    this->fitMorto = c.fitMorto;

    return *this;
}

int CromossomoMorto::getFitMorto() const {
    return this->fitMorto;
}

void CromossomoMorto::incFitMorto() {
    ++this->fitMorto;
}

void CromossomoMorto::decFitMorto() {
    --this->fitMorto;
}
