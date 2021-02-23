#include <iostream>
#include "Parser.h"
#include "Code.hpp"

// #define OUTPUT
std::string DecimalToBinary(const std::string& val, Parser& p)
{
    auto tmpStr = val;
    if(!(val[0] >= '0' && val[0] <= '9'))
    {
        if(p._symbolTable.contains(val))
        {
            if(val == "Loop")
            {
                std::cout << val << std::endl;
            }
            tmpStr = std::to_string(p._symbolTable.GetAddress(val));
        }
    }
    int n = std::stoi(tmpStr);
    std::string str;
    while(n!=0) { str = (n % 2 == 0 ? "0" : "1") + str; n/=2;}
    while(str.size() < 15)
    {
        str.insert(str.begin(), '0');
    }
    return str;
}
int main()
{
    std::ofstream file("/Users/fusionbolt/Desktop/tools/old/Pong1.hack", std::ios::out);

    Code d;
    Parser p("/Users/fusionbolt/Desktop/tools/old/Pong.asm");
    for (int i = 0; i < p._codes.size(); ++i)
    {
        p.advance();
        std::string binary;
        switch(p.commandType())
        {
            case Parser::Command::C_COMMAND:
            {
                std::cout << "C Command Binary" << std::endl;
                binary = "111" + d.comp(p.comp()) + d.dest(p.dest())
                        + d.jump(p.jump()) + "\r";
                break;
            }
            case Parser::Command::A_COMMAND:
            {
                std::cout << "A Command Binary" << std::endl;
                binary = "0" + DecimalToBinary(p.symbol(), p) + "\r";
                break;
            }
        }
#ifdef OUTPUT
        std::cout << binary << std::endl;
#endif
        file.write((binary).c_str(), binary.size());
    }
    return 0;
}