//
// Created by fusionbolt on 2019/9/12.
//

#ifndef PROJ6_PARSER_H
#define PROJ6_PARSER_H

#include <variant>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "SymbolTable.hpp"

class Parser
{
public:
    class Command
    {
    public:
        enum CMDType
        {
            A_COMMAND,
            C_COMMAND,
            L_COMMAND // 伪指令
        };
        using AorLCommand = std::string;
        struct CCommand
        {
            std::string dest, comp, jump;
        };

        std::variant<AorLCommand, CCommand> val;

        CMDType Type(){return _type;}

        void SetType(CMDType type){_type = type;}

    private:
        CMDType  _type;
    };

    Parser(const std::string &asmFileName);

    ~Parser();

    void advance();

    void PreProcessSourceFile();

    Command::CMDType commandType();

    std::string symbol();
    // dest=comp;jump
    std::string dest();

    std::string comp();

    std::string jump();

public:
    bool hasMoreCommands();

    void ACommandProcess(const std::string& Line);

    void CCommandProcess(const std::string& Line);

    void LCommandProcess(const std::string& Line);

    bool IsTrueACommand();

    bool IsTrueCCommand();

    bool IsTrueLCommand();

    std::string RemoveSpaceAndComment(const std::string& str);

    std::string RemoveSpace(const std::string& str);

    std::string RemoveComment(const std::string& str);

    std::vector<std::string> _codes;

    std::size_t _curCodeIndex;

    std::ifstream _asmFile;

    std::string _nowCode;

    std::size_t _varAddress;

    Command _command;

    SymbolTable _symbolTable;
};


#endif //PROJ6_PARSER_H
