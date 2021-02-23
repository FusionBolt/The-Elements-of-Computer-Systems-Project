//
// Created by fusionbolt on 2019/9/13.
//

#ifndef PROJ7_CODEWRITER_H
#define PROJ7_CODEWRITER_H

#include <string>
#include <stack>
#include <map>
#include <vector>
#include <fstream>

#include "Parser.h"

class CodeWriter
{
public:
    CodeWriter(const std::string &filePath);

    ~CodeWriter();

    void setFileName(const std::string &fileName);

    void WriteArithmetic(const std::string &command);

    void WritePushPop(Parser::Command &command, const std::string &segment, int index);

    void WriteLabel(const std::string& label);

    void WriteGoTo(const std::string& label);

    void WriteIf(const std::string& label);

    void WriteCall(const std::string& functionName, uint16_t numArgs);

    void WriteReturn();

    void WriteFunction(const std::string& functionName, uint16_t numLocals);

    void WriteInit();

    void MapInit();

    std::map<std::string, std::string> _segment;

    std::map<std::string, std::pair<std::string, std::string>> _operator;

    static std::map<std::string, uint16_t> _returnAddress;

    std::ofstream _writer;

    std::string _fileName;

    int _index;
};


#endif //PROJ7_CODEWRITER_H
