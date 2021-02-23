#include <iostream>
#include <string>
#include <fstream>

#include "CompilationEngine.h"
#include "JackAnalyzer.h"

int main()
{
//    CompilationEngine engine("/Users/fusionbolt/Desktop/nand2tetris/projects/11/Square/SquareGame.jack","result.xml");
//    engine.CompileClass();

    JackAnalyzer analyzer;
    auto files = analyzer.GetDicFileByFilter("/Users/fusionbolt/Desktop/nand2tetris/projects/11/Square/",
            [](const std::string& filePath){
        return filePath.substr(filePath.size() - 5, 5) == ".jack";
    });
    for(auto& file : files)
    {
        CompilationEngine engine(file, "result.xml");
        engine.CompileClass();
        // std::cout << val << std::endl;
    }

    return 0;
}