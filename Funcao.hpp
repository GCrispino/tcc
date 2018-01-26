#ifndef TCC_FUNCAO_H
#define TCC_FUNCAO_H

#include <vector>
#include <cmath>
#include <numeric>

class Funcao{
    private:
        double minGlobal,min,max;
        double (*funcao)(const std::vector<double> &);
        static double funcaoPadrao(const std::vector<double> &);
    
    public:
    
        Funcao(double minGlobal = 0,double min = 0, double max = 0,  double (*funcao)(const std::vector<double> &) = funcaoPadrao);
    
        double (* getFuncao())(const std::vector<double> &);
        double * getIntervalo() const;
        double getMinimoGlobal() const;
};

#endif //TCC_FUNCAO_H
