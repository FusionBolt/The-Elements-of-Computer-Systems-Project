//
// Created by fusionbolt on 2019/9/12.
//

#ifndef PROJ6_CODE_HPP
#define PROJ6_CODE_HPP

#include <string>
#include <map>

class Code
{
public:
    Code()
    {
        // M : memory
        // A : A register
        // D : D register
        destMap["null"] = "000";
        destMap["M"] = "001";
        destMap["D"] = "010";
        destMap["MD"] = "011";
        destMap["A"] = "100";
        destMap["AM"] = "101";
        destMap["AD"] = "110";
        destMap["AMD"] = "111";

        compMap["0"] = "0101010";
        compMap["1"] = "0111111";
        compMap["-1"] = "0111010";
        compMap["D"] = "0001100";
        compMap["A"] = "0110000";
        compMap["!D"] = "0001101";
        compMap["!A"] = "0110001";
        compMap["-D"] = "0001111";
        compMap["-A"] = "0110001";


        compMap["D+1"] = "0011111";
        compMap["A+1"] = "0110111";
        compMap["D-1"] = "0001110";
        compMap["A-1"] = "0110010";
        compMap["D+A"] = "0000010";
        compMap["D-A"] = "0010011";
        compMap["A-D"] = "0000111";
        compMap["D&A"] = "0000000";
        compMap["D|A"] = "0010101";

        compMap["1+D"] = "0011111";
        compMap["1+S"] = "0110111";
        compMap["A+D"] = "0000010";
        compMap["A&D"] = "0000000";
        compMap["A|D"] = "0010101";


        compMap["M"] = "1110000";
        compMap["!M"] = "1110001";
        compMap["-M"] = "1110001";
        compMap["M+1"] = "1110111";
        compMap["M-1"] = "1110010";
        compMap["D+M"] = "1000010";
        compMap["D-M"] = "1010011";
        compMap["M-D"] = "1000111";
        compMap["D&M"] = "1000000";
        compMap["D|M"] = "1010101";

        compMap["1+M"] = "1110111";
        compMap["M+D"] = "1000010";
        compMap["M&D"] = "1000000";
        compMap["M|D"] = "1010101";

        jumpMap["null"] = "000";
        jumpMap["JGT"] = "001";
        jumpMap["JEQ"] = "010";
        jumpMap["JGE"] = "011";
        jumpMap["JLT"] = "100";
        jumpMap["JNE"] = "101";
        jumpMap["JLE"] = "110";
        jumpMap["JMP"] = "111";
    }
    std::string dest(const std::string &symbol)
    {
        return destMap[symbol];
    }
    std::string comp(const std::string &symbol)
    {
        return compMap[symbol];
    }
    std::string jump(const std::string &symbol)
    {
        return jumpMap[symbol];
    }
    std::map<std::string, std::string> destMap, compMap, jumpMap;
};


#endif //PROJ6_CODE_HPP
