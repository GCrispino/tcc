cmake_minimum_required(VERSION 3.9)
project(TCC)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-fopenmp")

add_executable(TCC
        Populacao.cpp
        Populacao.h
        Cromossomo.cpp
        Cromossomo.hpp
        main.cpp

        Funcao.cpp
        Funcao.hpp
        Funcoes.hpp
        )
