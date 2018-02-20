cmake_minimum_required(VERSION 3.9)
project(TCC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fopenmp")

add_executable(Convencional
        Populacao.cpp
        Populacao.h
        Cromossomo.cpp
        Cromossomo.hpp
        mainConvencional.cpp

        Funcao.cpp
        Funcao.hpp
        Funcoes.hpp
        CromossomoMorto.cpp CromossomoMorto.hpp PopulacaoTransformacao.cpp PopulacaoTransformacao.h)


add_executable(Paralelo
        Populacao.cpp
        Populacao.h
        Cromossomo.cpp
        Cromossomo.hpp
        Migracao.cpp
        Migracao.h
        mainParalelo.cpp

        Funcao.cpp
        Funcao.hpp
        Funcoes.hpp
        CromossomoMorto.cpp CromossomoMorto.hpp PopulacaoTransformacao.cpp PopulacaoTransformacao.h)


add_executable(NaoConvencional
        Populacao.cpp
        Populacao.h
        Cromossomo.cpp
        Cromossomo.hpp
        mainNaoConvencional.cpp

        Funcao.cpp
        Funcao.hpp
        Funcoes.hpp
        CromossomoMorto.cpp CromossomoMorto.hpp PopulacaoTransformacao.cpp PopulacaoTransformacao.h)
