#include "Funcao.hpp"
#include <iostream>

double Funcao::funcaoPadrao(const std::vector<double> &x){
    return 0.0;
}


Funcao::Funcao(double minGlobal,double min, double max,  double (*funcao)(const std::vector<double> &))
:minGlobal(minGlobal),min(min),max(max),funcao(funcao){}
    
double (*Funcao::getFuncao())(const std::vector<double> &){
    return this->funcao;
}

double * Funcao::getIntervalo() const{
	double *intervalo = new double[2];
	
	intervalo[0] = this->min;
	intervalo[1] = this->max;

	return intervalo;
}

double Funcao::getMinimoGlobal() const {return this->minGlobal;}




