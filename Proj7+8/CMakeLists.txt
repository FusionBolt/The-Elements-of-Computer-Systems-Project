cmake_minimum_required(VERSION 3.14)
project(Proj7)

set(CMAKE_CXX_STANDARD 17)

add_executable(Proj7 main.cpp Parser.cpp Parser.h CodeWriter.cpp CodeWriter.h AsmDefine.hpp)