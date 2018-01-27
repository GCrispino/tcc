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

Funcao::Funcao(const Funcao &f)
:minGlobal(f.minGlobal),min(f.min),max(f.max),funcao(f.funcao){}

Funcao& Funcao::operator = (const Funcao &f){
	if (this == &f)
		return *this;

	this->minGlobal = f.minGlobal;
	this->min = f.min;
	this->max = f.max;
	this->funcao = f.funcao;

}

double * Funcao::getIntervalo() const{
	double *intervalo = new double[2];
	
	intervalo[0] = this->min;
	intervalo[1] = this->max;

	return intervalo;
}

double Funcao::getMinimoGlobal() const {return this->minGlobal;}




