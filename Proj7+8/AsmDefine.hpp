//
// Created by fusionbolt on 2019/9/14.
//

#ifndef PROJ7_ASMDEFINE_HPP
#define PROJ7_ASMDEFINE_HPP

#include <string>

constexpr auto TRUE = 0xFFFF;
constexpr auto FALSE = 0x0000;

inline std::string EspAdd()
{
    return "@SP\nM=M+1\n";
}

inline std::string EspSub()
{
    return "@SP\nM=M-1\n";
}

inline std::string LoadSegValToD(const std::string& seg, uint16_t offset)
{
    return "@" + seg +"\nA=M\nD=A\n"
           "@" + std::to_string(offset) + "\n"
           "A=D+A\n"
           "D=M\n";
}

inline std::string LoadValToD(int16_t constantVal)
{
    return "@" + std::to_string(constantVal) + "\nD=A\n";
}

inline std::string SetEspPosValByD()
{
    return "@SP\nA=M\nM=D\n";
}

inline std::string SaveStackTopValToD()
{
    return "A=M\nD=M\n";
}

inline std::string BinaryOperation(const std::string& operatorSymbol)
{
    return EspSub() +
        SaveStackTopValToD() +
        // "A=M\nD=M\n" +
        EspSub() +
        "A=M\nM=M" + operatorSymbol + "D\n" +
        EspAdd();
}

inline std::string LogicOperation(const std::string& operatorSymbol, int index)
{
    auto i = std::to_string(index);
    return EspSub() +
        "A=M\nD=M\n" +
        EspSub() +
        "A=M\nD=M-D\n" // x=m y=d     A is now address;
        "@TRUE" + i + "\n"
        "D;" + operatorSymbol + "\n" + // JGT JLT JEQ
        "@SP\nA=M\n"
        "M=0\n"
        "@END" + i + "\n"
        "0;JMP\n"
        "(TRUE" + i + ")\n"
        "@SP\nA=M\n"
        "M=-1\n"
        "(END" + i + ")\n" +
        EspAdd();
}

inline std::string UnaryOperation(const std::string& operatorSymbol)
{
    return EspSub() +
        "A=M\nM=" + operatorSymbol + "M\n" +
        EspAdd();
}






inline std::string LoadSymbolToA(const std::string& symbol)
{
    return "@" + symbol + "\n";
}

inline std::string GenerateSymbol(const std::string& symbol)
{
    return "(" + symbol + ")\n";
}

inline std::string GenerateGoToFun(const std::string& functionName)
{
    return LoadSymbolToA(functionName) + "0;JMP\n";
}

inline std::string PushSymbolVal(const std::string& symbol)
{
    return LoadSymbolToA(symbol) +
           "D=A\n" +
           SetEspPosValByD() +
           EspAdd();
}

inline std::string PushRegVal(const std::string& reg)
{
    return LoadSymbolToA(reg) +
//           "A=M\n"
           "D=M\n" +
           SetEspPosValByD() +
           EspAdd();
}

inline std::string PushConstantVal(int16_t val)
{
    return LoadValToD(val) +
           SetEspPosValByD() +
           EspAdd();
}

#endif //PROJ7_ASMDEFINE_HPP
