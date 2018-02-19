#ifndef TCC_FUNCOES
#define TCC_FUNCOES

#define _USE_MATH_DEFINES
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <numeric>
#include <cmath>
#include "Funcao.hpp"


namespace Funcoes{

    double func_schaffer2(const std::vector<double> &x){
        double
                x1 = x[0],x2 = x[1],
                p1 = pow((sin(x1 * x1 - x2 * x2)),2) - .5,
                p2 = pow((1 + 0.001 * (x1 * x1 + x2 * x2)),2);

        return 0.5 + p1 / p2;
    }

    double func_booth(const std::vector<double> &x){
        double
                x1 = x[0],x2 = x[1],
                p1 = pow((x1 + 2 * x2 -7),2) ,
                p2 = pow((2 * x1 + x2 - 5),2);

        return p1 + p2;
    }

    double func_ackley(const std::vector<double> &x){
        double d = (double)x.size();
        double p1,p2,soma1,soma2;
        const double pi = M_PI;

        for (int i = 0; i < d;++i){
            soma1 += pow(x[i],2);
            soma2 +=  (double)cos(2.0 * pi * x[i]);
        }

        p1 = -20 * exp(-.2f * sqrt(soma1  / d));
        p2 = -exp(soma2 / d) + exp(1) + 20;

        return p1 + p2;
    }


    double func_griewank(const std::vector<double> &x){
        double
                d = x.size(),
                soma,
                prod = 1;

        auto lambda1 = [](double a,double b){return a + pow(b,2);};

        soma = std::accumulate(x.begin(),x.end(),0.0,lambda1);

        for (unsigned int i = 0;i < d;++i)
            prod *= cos(x[i] / sqrt(i + 1));

        return (soma / 4000.0) - prod + 1;
    }

    double func_eggholder(const std::vector<double> &input){
        const double
                &x = input[0],&y = input[1],
                p1 = -(y + 47) * sin(sqrt(abs(y + (x / 2.0) + 47))),
                p2 = x * sin(sqrt(abs(x - (y + 47))));


        return p1 - p2;
    }

    double func_dropwave(const std::vector<double> &x){
        double
                x1 = x[0],x2 = x[1],
                p1,p2;


        p1 = 1 + cos(12.0 * sqrt(x1 * x1 + x2 * x2));
        p2 = .5 * (x1 * x1 + x2 * x2) + 2;

        return -(p1 / p2);
    }


    double func_rosenbrock(const std::vector<double> &x){
        double soma = 0.0;

        for (unsigned int i = 0;i < x.size() - 1;++i)
            soma += 100 * pow((x[i + 1] - x[i] * x[i]),2) + pow((x[i] - 1),2);
        
        return soma;
    }


    Funcao
            schaffer2(0.0,-1000.0,1000.0    ,func_schaffer2),
            dropwave(-1.0,-5.12,5.12,func_dropwave),
            booth(0.0,-10,10,func_booth),
            ackley(0.0,-32,32,func_ackley),
            griewank(0.0,-600,600,func_griewank),
            rosenbrock(0,-5,10,func_rosenbrock),
            eggholder(-959.6407,-512,512,func_eggholder);
};

#endif
