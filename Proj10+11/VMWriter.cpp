//
// Created by fusionbolt on 2019/9/24.
//

#include "VMWriter.h"

VMWriter::VMWriter()
{

}

VMWriter::~VMWriter()
{
    _writer.close();
}

void VMWriter::WritePush(const std::string& seg, std::size_t index)
{
    outPut = "push " + seg + " " + std::to_string(index) + "\n";
    _writer.write(outPut.c_str(), outPut.size());
}

void VMWriter::WritePop(const std::string& seg, std::size_t index)
{
    outPut = "pop " + seg + " " + std::to_string(index) + "\n";
    _writer.write(outPut.c_str(), outPut.size());
}

void VMWriter::WriteArithmetic(const std::string& cmd)
{
    outPut = cmd + "\n";
    _writer.write(outPut.c_str(), outPut.size());
}

void VMWriter::WriteLabel(const std::string &label)
{
    outPut = "label " + label + "\n";
    _writer.write(outPut.c_str(), outPut.size());
}

void VMWriter::WriteGoto(const std::string &label)
{
    outPut = "goto " + label + "\n";
    _writer.write(outPut.c_str(), outPut.size());
}

void VMWriter::WriteIf(const std::string &label)
{
    outPut = "if-goto " + label + "\n";
    _writer.write(outPut.c_str(), outPut.size());
}

void VMWriter::WriteCall(const std::string &label, std::size_t nArgs)
{
    outPut = "call " + label + " " + std::to_string(nArgs) + "\n";
    _writer.write(outPut.c_str(), outPut.size());
}

void VMWriter::WriteFunction(const std::string &label, std::size_t nArgs)
{
    outPut = "function " + label + " " + std::to_string(nArgs) + "\n";
    _writer.write(outPut.c_str(), outPut.size());
}

void VMWriter::WriteReturn()
{
    outPut = "return\n";
    _writer.write(outPut.c_str(), outPut.size());
}

void VMWriter::WriteOutPut()
{
    _writer.write(outPut.c_str(), outPut.size());
}

void VMWriter::SetOutPutFileName(const std::string &fileName)
{
    _writer = std::ofstream(fileName);
}
