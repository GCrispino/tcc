#ifndef TCC_FUNCAO_H
#define TCC_FUNCAO_H

#include <vector>
#include <string>
#include <cmath>
#include <numeric>

class Funcao{
    private:
        std::string nome;
        double minGlobal,min,max;
        double (*funcao)(const std::vector<double> &);
        static double funcaoPadrao(const std::vector<double> &);
    
    public:
    
        Funcao(const std::string &nome = "Funcao",double minGlobal = 0,double min = 0, double max = 0,  double (*funcao)(const std::vector<double> &) = funcaoPadrao);
        Funcao(const Funcao &);
        Funcao operator = (const Funcao &);

        double (* getFuncao())(const std::vector<double> &);
        const std::string getNome() const;
        double * getIntervalo() const;
        double getMinimoGlobal() const;
};

#endif //TCC_FUNCAO_H
