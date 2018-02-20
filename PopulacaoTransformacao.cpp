//
// Created by Crispino on 2/18/2018.
//

#include "PopulacaoTransformacao.h"

/**
 * PROBLEMA!
 * - variável taxaInfeccao não é usada
 */


PopulacaoTransformacao::PopulacaoTransformacao(
        unsigned int tamanho, unsigned int tamanhoPopulacaoMortos,
        float txMutacao, float txCruzamento, unsigned int taxaInfeccao,
        float desvioPadrao, const Funcao &funcaoFitness)
:Populacao(tamanho,txMutacao,txCruzamento,taxaInfeccao,desvioPadrao,funcaoFitness)
{
    this->tamanhoPopulacaoMortos = tamanhoPopulacaoMortos;
    this->taxaInfeccao = taxaInfeccao;
}

void PopulacaoTransformacao::selecaoSobreviventes(const std::vector<Cromossomo> &filhos) {
    const Cromossomo pior = this->getElemMinFitness();

    for (int i = 0; i < filhos.size(); i += 2) {
        const Cromossomo
                &filho1 = filhos[i],
                &filho2 = filhos[i + 1];


        const Cromossomo
                *escolhido = filho1.getFitness() < filho2.getFitness() ? &filho1 : &filho2,
                *outro = escolhido->getFitness() == filho1.getFitness() ? &filho2 : &filho1;

        this->cromossomos[this->iElemMinFitness] = *escolhido;
        this->mortos.push_back(*outro);

        this->iElemMinFitness = this->achaIndicePiorFitness();
    }
}

void PopulacaoTransformacao::recombinacao(){
/*
     * Seleciona um aleatório da população
     * Seleciona um aleatório da população dos mortos
     * faz o cruzamento simples
     *      se o filho for melhor, incrementa o valor de fitMorto
     *      se não, decrementa
     * ao fim, se a população estiver cheia, tira os indivíduos com os menores fitMorto até ela alcançar o seu tamanho máximo
     */

    for (unsigned int i = 0; i < this->taxaInfeccao; ++i) {
        std::uniform_int_distribution<int>
                distPopulacao (0,this->tamanho - 1),
                distMortos (0,this->mortos.size() - 1);

        unsigned int
                iCromossomo = distPopulacao(*(this->gen)),
                iMorto = distMortos(*(this->gen));

        CromossomoMorto &morto = this->mortos[iMorto];

        Cromossomo
                &c = this->cromossomos[iCromossomo],
                novo = c.crossoverSimples(morto);

        if (novo.getFitness() < c.getFitness()){
            //decrementa fitMorto
            morto.decFitMorto();
        }
        else{
            //incrementa fitMorto
            morto.incFitMorto();
        }
    }


    unsigned int tamAtualPopulacaoMortos = this->mortos.size();

    if (tamAtualPopulacaoMortos > this->tamanhoPopulacaoMortos){
        //ordenar população baseado no fitmorto
        std::sort(this->mortos.begin(),this->mortos.end(),[](CromossomoMorto &c1, CromossomoMorto &c2){
            return c1.getFitMorto() > c2.getFitMorto();
        });

        for (auto &morto: this->mortos){
            morto.calcularFitness();
        }

        //excluir os menores
        std::vector<CromossomoMorto>::iterator inicioRemocao = this->mortos.begin() + this->tamanhoPopulacaoMortos;
        this->mortos.erase(inicioRemocao,this->mortos.end());

    }
}






