cmake_minimum_required(VERSION 3.9)
project(TCC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fopenmp")


add_executable(Paralelo
        Populacao.cpp
        Populacao.h
        Cromossomo.cpp
        Cromossomo.hpp
        Migracao.cpp
        Migracao.h
        main.cpp

        Funcao.cpp
        Funcao.hpp
        Funcoes.hpp
        CromossomoMorto.cpp CromossomoMorto.hpp PopulacaoTransformacao.cpp PopulacaoTransformacao.h Algoritmos.hpp Resultado.h)
