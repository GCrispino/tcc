#include <iostream>
#include <chrono>
#include "PopulacaoTransformacao.h"
#include "Funcoes.hpp"

int main(){

    PopulacaoTransformacao p(100,50,0.05,0.9,30,1.55,Funcoes::eggholder);

    int
        i = 0,
        nGeracoes = 1000,
        nParesPaisASelecionar = 15,tamTorneio = 2;

    std::chrono::steady_clock::time_point comeco,fim;
    std::chrono::steady_clock::duration tempo;

    comeco = std::chrono::steady_clock::now();

    p.inicializacao();
    p.calcularFitness();
    std::cout << "iniciou" << std::endl;
    do {
        std::vector<Cromossomo> pais,filhos;
        pais = p.selecaoPais(nParesPaisASelecionar,tamTorneio);

        filhos = p.gerarFilhos(pais);

        p.selecaoSobreviventes(filhos);

        p.calcularFitness();
        
        if (i % 100 == 0) {
            std::cout << "Geracao: " << i << '\t';
            std::cout << "Melhor fitness: " << p.getElemMaxFitness().getFitness() << '\t';
            std::cout << "Media do fitness: " << p.getMediaFitness();
            std::cout << std::endl;
        }
    } while (++i < nGeracoes/* && !p.verificarParada()*/);

    const Cromossomo &melhor = p.getElemMaxFitness();

    fim = std::chrono::steady_clock::now();
    tempo = fim - comeco;

    double nseconds = double(tempo.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;

    std::cout << "Terminou: tempo em " << nseconds << std::endl;
    std::cout << "Numero de geracoes: " << i << std::endl;
    std::cout << "Melhor individuo: " << melhor << std::endl;
    std::cout << "Media do fitness da populacao: " << p.getMediaFitness();

    return 0;
}
