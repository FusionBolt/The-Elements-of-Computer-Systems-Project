//
// Created by fusionbolt on 2019/9/13.
//

#include "CodeWriter.h"
#include "AsmDefine.hpp"

std::map<std::string, uint16_t> CodeWriter::_returnAddress = {};

CodeWriter::CodeWriter(const std::string& filePath):
        _writer("/Users/fusionbolt/Desktop/nand2tetris/projects/08/FunctionCalls/FibonacciElement/FibonacciElement.asm", std::ios::app), _index(0)
{
    WriteInit();

    std::cout << "write file" << filePath << std::endl;

    MapInit();

    setFileName(filePath);
}

CodeWriter::~CodeWriter()
{
    _writer.close();
}

void CodeWriter::setFileName(const std::string &fileName)
{
    auto index = fileName.find_last_of("/");
    auto index2 = fileName.find_last_of(".");
    _fileName = fileName.substr(index + 1, index2-(index+1));
    std::cout << _fileName << std::endl;
}

void CodeWriter::WriteArithmetic(const std::string &command)
{
    std::string outPut;
    if(_operator[command].second == "Binary")
    {
        outPut = BinaryOperation(_operator[command].first);
    }
    if(_operator[command].second == "Unary")
    {
        outPut = UnaryOperation(_operator[command].first);
    }
    if(_operator[command].second == "Logic")
    {
        outPut = LogicOperation(_operator[command].first, _index++);
    }
    _writer.write(outPut.c_str(), outPut.size());
    std::cout << outPut << std::endl;
}

void CodeWriter::WritePushPop(Parser::Command &command, const std::string &segment, int index)
{
    if(command.type != Parser::Command::C_PUSH &&
        command.type != Parser::Command::C_POP)
    {
        std::cout << "Not Push or Pop Command!!!!!!!!!" << std::endl;
    }
    std::string outPut;
    if(command.cmd == "push")
    {
        if(command.arg1 == "constant")
        {
//            outPut = LoadValToD(command.arg2) +
//                     SetEspPosValByD() +
//                     EspAdd();
            outPut = PushConstantVal(command.arg2);
        }
        else if(command.arg1 == "pointer" || command.arg1 == "temp")
        {
            outPut = "@" + _segment[command.arg1] + "\n"
                     "D=A\n"
                     "@" + std::to_string(command.arg2) + "\n"
                     "A=D+A\n"
                     "D=M\n" +
                     SetEspPosValByD() +
                     EspAdd();
        }
        else if(command.arg1 == "static")
        {
            // file process
            outPut = "@" + _fileName + "." + std::to_string(command.arg2) + "\r"
                     "D=M\n"
                     "@SP\n"
                     "A=M\n"
                     "M=D\n" +
                     EspAdd();
        }
        else
        {
            outPut = LoadSegValToD(_segment[command.arg1], command.arg2) +
                     SetEspPosValByD() +
                     EspAdd();
        }
    }
    else if(command.cmd == "pop")
    {
        std::string ins = "D=M\n";
        // D = M get target address from M[A] (save pointer)
        // D = A get target address from A (direct saving)
        if(command.arg1 == "temp" || command.arg1 == "pointer")
        {
            ins = "D=A\n";
        }
        if(command.arg1 == "static")
        {
            outPut = EspSub() +
                    "A=M\n"
                    "D=M\n"
                    "@" + _fileName + "." + std::to_string(command.arg2) + "\n"
                    "M=D\n";
        }
        else
        {
            outPut = "@" + _segment[command.arg1] + "\n" + ins +
                     //"D=M\n"
                     "@" + std::to_string(command.arg2) + "\n"
                     "D=D+A\n"
                     "@R13\n"
                     "M=D\n" + // save top to R13
                     EspSub() +
                     SaveStackTopValToD() +
                     "@R13\n"
                     "A=M\n"
                     "M=D\n";
        }
    }
    _writer.write(outPut.c_str(), outPut.size());
}

void CodeWriter::MapInit()
{
    _segment["argument"] = "ARG";
    _segment["local"] = "LCL";
    _segment["static"] = "16";
    _segment["this"] = "THIS";
    _segment["that"] = "THAT";
    _segment["pointer"] = "R3";
    _segment["temp"] = "R5";

    _operator["add"] = std::make_pair("+", "Binary");
    _operator["sub"] = std::make_pair("-", "Binary");
    _operator["neg"] = std::make_pair("-", "Unary");
    _operator["eq"] = std::make_pair("JEQ", "Logic");
    _operator["gt"] = std::make_pair("JGT", "Logic");
    _operator["lt"] = std::make_pair("JLT", "Logic");
    _operator["and"] = std::make_pair("&", "Binary");
    _operator["or"] = std::make_pair("|", "Binary");
    _operator["not"] = std::make_pair("!", "Unary");
}

void CodeWriter::WriteLabel(const std::string &label)
{
    std::string outPut = GenerateSymbol(label);
    _writer.write(outPut.c_str(), outPut.size());
    std::cout << "WriteLable:" << label << std::endl;
}

void CodeWriter::WriteGoTo(const std::string &label)
{
    std::string outPut = GenerateGoToFun(label);
    _writer.write(outPut.c_str(), outPut.size());
    std::cout << "WriteGoTo:" << label << std::endl;
}
void CodeWriter::WriteIf(const std::string &label)
{
    std::string outPut = EspSub() +
                         SaveStackTopValToD() +
                         LoadSymbolToA(label) +
                         "D;JNE\n";
    _writer.write(outPut.c_str(), outPut.size());
    std::cout << "Write If:" << label << std::endl;
    // pop
    // 等于0则跳转
}
void CodeWriter::WriteCall(const std::string &functionName, uint16_t numArgs)
{
    // save:return address;LCL ARG THIS THAT
    if(_returnAddress.find(functionName) != _returnAddress.end())
    {
        _returnAddress[functionName] += 1;
    }
    else
    {
        _returnAddress[functionName] = 0;
    }
    auto funSymbol = [&](){
        return functionName + std::to_string(_returnAddress[functionName]);
    };
    std::string outPut = PushSymbolVal(funSymbol()) +
                         PushRegVal("LCL") +
                         PushRegVal("ARG") +
                         PushRegVal("THIS") +
                         PushRegVal("THAT") +
                         "@SP\n"
                         "D=M\n"
                         "@5\n"
                         "D=D-A\n"
                         "@" + std::to_string(numArgs) + "\n"
                         "D=D-A\n"
                         "@ARG\n"
                         "M=D\n" // n个参数被压入栈以后调用f

                         "@SP\n"
                         "D=M\n"
                         "@LCL\n"
                         "M=D\n" +
                         // ARG=SP-n-5
                         // LCL=SP
                         GenerateGoToFun(functionName) +
                         GenerateSymbol(funSymbol());
    _writer.write(outPut.c_str(), outPut.size());
}

void CodeWriter::WriteReturn()
{
    auto PopToArg = [](const std::string& symbol){
        return "@R13\n"
               "D=M-1\n" // val, not pointer
               "AM=D\n"
               "D=M\n"
               "@" + symbol + "\n"
               "M=D\n";
    };
    std::string outPut = "@LCL\n"
                         "D=M\n"
                         "@R13\n"
                         "M=D\n" // save LCL to R13
                         "@5\n" // LCL is base
                         "A=D-A\n"
                         "D=M\n"
                         "@R14\n"
                         "M=D\n" + // save ret address to R14
                         EspSub() +
                         "A=M\n"
                         "D=M\n" // save ret val to arg1 position
                         "@ARG\n"
                         "A=M\n"
                         "M=D\n"
                         "@ARG\n"
                         "D=M+1\n"
                         "@SP\n"
                         "M=D\n" +
                         PopToArg("THAT") +
                         PopToArg("THIS") +
                         PopToArg("ARG") +
                         PopToArg("LCL") +
                         "@R14\n"
                         "A=M\n"
                         "0;JMP\n";
    _writer.write(outPut.c_str(), outPut.size());
    std::cout << "Write Return" << std::endl;
}

void CodeWriter::WriteFunction(const std::string &functionName, uint16_t numLocals)
{
    std::string outPut = GenerateSymbol(functionName);
    for (std::size_t i = 0; i < numLocals; ++i)
    {
        outPut += PushConstantVal(0);
    }
    _writer.write(outPut.c_str(), outPut.size());
}

void CodeWriter::WriteInit()
{
    // Esp init
    std::string outPut = "@261\nD=A\n"
                         "@SP\nM=D\n" // Esp init
                         "@Sys.init\n" // call
                         "0;JMP\n";
    _writer.write(outPut.c_str(), outPut.size());
}
