//
// Created by fusionbolt on 2019/9/12.
//

#include "Parser.h"

Parser::Parser(const std::string &asmFileName):
        _asmFile(asmFileName, std::ios::in),
        _varAddress(16),
        _curCodeIndex(0)
{
    if (!_asmFile.is_open()) {
        std::cout << "file open failed";
    }
    PreProcessSourceFile();
}

void Parser::advance()
{
    if(!hasMoreCommands())
    {
        return;
    }

    std::string nowLine = _codes[_curCodeIndex++];

    bool syntaxTrue = true;
    switch(nowLine[0])
    {
        // A
        case '@':
            if(!IsTrueACommand())
            {
                syntaxTrue = false;
                break;
            }
            ACommandProcess(nowLine);
            break;
        // L
        case '(':
            if(!IsTrueLCommand())
            {
                syntaxTrue = false;
                break;
            }
            LCommandProcess(nowLine);
            break;
        // C
        default:
            if(!IsTrueCCommand())
            {
                syntaxTrue = false;
                break;
            }
            CCommandProcess(nowLine);
            break;
    }
    if(!syntaxTrue)
    {
        std::cout << nowLine << "syntax error" << std::endl;
    }

#ifdef OUTPUT
    std::cout << nowLine << std::endl;
    std::cout << "type:" << _command.Type() << std::endl;
    std::cout << "--------" << std::endl;
    return;
#endif
}

std::string Parser::RemoveSpaceAndComment(const std::string &str)
{
    return RemoveSpace(RemoveComment(str));
}

std::string Parser::RemoveSpace(const std::string &str)
{
    auto tmpStr = str;
    auto iter = remove_if(tmpStr.begin(), tmpStr.end(), ::isspace);
    tmpStr.erase(iter, tmpStr.end());
    return tmpStr;
}

std::string Parser::RemoveComment(const std::string &str)
{
    return str.substr(0, str.find_first_of("//"));
}

        bool Parser::hasMoreCommands()
        {
            if(_curCodeIndex >= _codes.size())
            {
        return false;
    }
    return true;
}

void Parser::PreProcessSourceFile()
{
    std::size_t codeAddress = 0;
    std::size_t varAddress = 16;
    std::string tempStr;
    std::string prevSymbol;
    bool saveSymbol = false;
    while(std::getline(_asmFile, tempStr))
    {
        //std::cout << tempStr << std::endl;
        // _sourceCode += RemoveSpaceAndComment(tempStr) + "\n";
        auto code = RemoveSpaceAndComment(tempStr);
        if(code == "")
        {
            continue;
        }
        if(code[0] == '(')
        {
            // L_COMMAND codeAddress don't need ++
#ifdef OUTPUT
            std::cout << code << " " << codeAddress << std::endl;
#endif
            _symbolTable.addEntry(code.substr(1, code.size() - 2), codeAddress);
            continue;
        }
        _codes.push_back(code);
        ++codeAddress;
    }
}

void Parser::ACommandProcess(const std::string& Line)
{
    auto symbol = Line.substr(1);
    if((!_symbolTable.contains(symbol)) && (!(symbol[0] >= '0' && symbol[0] <='9')))
    {
        _symbolTable.addEntry(Line.substr(1), _varAddress++);
    }
    _command.val = Line.substr(1);
    _command.SetType(Command::A_COMMAND);
}

void Parser::CCommandProcess(const std::string& Line)
{
    std::string dest, comp, jump;
    auto destIndex = Line.find_first_of("=");
    auto jumpIndex = Line.find_first_of(";");

    if (destIndex != -1)
    {
        dest = Line.substr(0, destIndex);
    }
    else
    {
        dest = "null";
        destIndex = 0;
    }

    if(jumpIndex != -1)
    {
        jump = Line.substr(jumpIndex + 1);
    }
    else
    {
        jump = "null";
        jumpIndex = Line.size();
    }

    std::size_t compIndex;
    if(destIndex == 0)
    {
        compIndex = 0;
    }
    else
    {
        compIndex = destIndex + 1;
    }
    comp = Line.substr(compIndex, jumpIndex - destIndex);

    _command.SetType(Command::C_COMMAND);
    _command.val = Command::CCommand{dest, comp, jump};
}

void Parser::LCommandProcess(const std::string& Line)
{
    auto symbol = Line.substr(1, Line.size() - 2);
    _command.val = symbol;
    _command.SetType(Command::L_COMMAND);
}

bool Parser::IsTrueCCommand()
{
    // A M D + 1
    // [\s*= | \s*; JMP]
    return true;
}

Parser::~Parser()
{
    _asmFile.close();
}

bool Parser::IsTrueACommand()
{
    return true;
}

bool Parser::IsTrueLCommand()
{
    return true;
}

Parser::Command::CMDType Parser::commandType()
{
    return _command.Type();
}

std::string Parser::symbol()
{
    if(commandType() == Command::A_COMMAND)
    {
        return std::get<Command::AorLCommand >(_command.val);
    }
    else if(commandType() == Command::L_COMMAND)
    {
        return "(" + std::get<Command::AorLCommand>(_command.val) + ")";
    }
    else
    {
        return "";
    }
    // 返回形如@Xxx或者Xxx的当前命令符号或者十进制
    //commandType()是A_COMMAND或者L_COMMAND才能使用
}

std::string Parser::dest()
{
    if(commandType() != Command::C_COMMAND)
    {
        return "";
    }
    return std::get<Command::CCommand>(_command.val).dest;
}

std::string Parser::comp()
{
    if(commandType() != Command::C_COMMAND)
    {
        return "";
    }
    return std::get<Command::CCommand>(_command.val).comp;
}

std::string Parser::jump()
{
    if(commandType() != Command::C_COMMAND)
    {
        return "";
    }
    return std::get<Command::CCommand>(_command.val).jump;
}
