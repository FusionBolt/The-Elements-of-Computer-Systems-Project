//
// Created by fusionbolt on 2019/9/13.
//

#ifndef PROJ7_PARSER_H
#define PROJ7_PARSER_H

#include <string>
#include <fstream>
#include <iostream>

class Parser
{
public:
    struct Command
    {
        enum CMDType
        {
            C_ARITHMETIC,
            C_PUSH,
            C_POP,
            C_LABEL,
            C_GOTO,
            C_IF,
            C_FUNCTION,
            C_RETURN,
            C_CALL
        };
        std::string arg1;
        int16_t arg2;
        CMDType type;
        std::string cmd;
    };
    Parser(const std::string &srcFilePath);

    bool hasMoreCommands();

    void advance();

    Command::CMDType commandType();

    std::string arg1();

    int arg2();

public:
    std::string RemoveSpaceAndComment(const std::string& str);

    std::string RemoveSpace(const std::string& str);

    std::string RemoveComment(const std::string& str);

    Command _command;
    std::ifstream _srcFile;

    std::string _srcFileName;
};


#endif //PROJ7_PARSER_H
