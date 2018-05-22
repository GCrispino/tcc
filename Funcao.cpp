#include "Funcao.hpp"
#include <iostream>

double Funcao::funcaoPadrao(const std::vector<double> &x){
    return 0.0;
}


Funcao::Funcao(const std::string &nome,double minGlobal,double min, double max,  double (*funcao)(const std::vector<double> &))
:nome(nome),minGlobal(minGlobal),min(min),max(max),funcao(funcao){}

Funcao::Funcao(const Funcao &f):nome(f.nome),minGlobal(f.minGlobal),min(f.min),max(f.max),funcao(f.funcao){}

Funcao Funcao::operator = (const Funcao &f){
	if (this == &f)
		return *this;

	this->nome = f.nome;
	this->minGlobal = f.minGlobal;
	this->min = f.min;
	this->max = f.max;
	this->funcao = f.funcao;

	return *this;
}


double (*Funcao::getFuncao())(const std::vector<double> &){
    return this->funcao;
}

const std::string Funcao::getNome() const {
    return this->nome;
}

double * Funcao::getIntervalo() const{
	double *intervalo = new double[2];
	
	intervalo[0] = this->min;
	intervalo[1] = this->max;

	return intervalo;
}

double Funcao::getMinimoGlobal() const {return this->minGlobal;}




