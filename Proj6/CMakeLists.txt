cmake_minimum_required(VERSION 3.14)
project(Proj6)

set(CMAKE_CXX_STANDARD 17)

add_executable(Proj6 main.cpp Code.hpp SymbolTable.hpp Parser.cpp Parser.h Utils.h)