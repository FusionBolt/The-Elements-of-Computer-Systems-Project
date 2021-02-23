cmake_minimum_required(VERSION 3.14)
project(Proj10)

set(CMAKE_CXX_STANDARD 17)

add_executable(Proj10 main.cpp JackAnalyzer.cpp JackAnalyzer.h JackTokenizer.cpp JackTokenizer.h CompilationEngine.cpp CompilationEngine.h SymbolTable.cpp SymbolTable.h VMWriter.cpp VMWriter.h)