//
// Created by fusionbolt on 2019/9/13.
//

#include "Parser.h"

Parser::Parser(const std::string &srcFilePath):
        _srcFile(srcFilePath, std::ios::in),
        _srcFileName(srcFilePath)
{
    if (!_srcFile.is_open()) {
        std::cout << "file:" << srcFilePath << " open failed";
    }
}

bool Parser::hasMoreCommands()
{
    return false;
}

void Parser::advance()
{
    std::string tmpStr;
    std::getline(_srcFile, tmpStr);
    tmpStr = RemoveComment(tmpStr);
    if(tmpStr.size() == 1 || tmpStr.size() == 0)
    {
        _command.cmd = "";
        return;
    }
    if(tmpStr.find("\r") != -1)
    {
        tmpStr = tmpStr.substr(0, tmpStr.size() - 1);
    }

    std::cout << "tmpStr:" << tmpStr << std::endl;
    std::string token1, token2, token3;
    if((tmpStr.find("push") != -1) ||
            (tmpStr.find("pop") != -1) ||
            (tmpStr.find("function") != -1) ||
            (tmpStr.find("call") != -1))
    {
        auto index1 = tmpStr.find_first_of(" ");
        // auto index2 = tmpStr.find_last_of(" ");
        auto index2 = tmpStr.find(" ", index1 + 1);
        token1 = tmpStr.substr(0, index1);
        token2 = tmpStr.substr(index1 + 1, index2 - (index1 + 1));
        token3 = tmpStr.substr(index2 + 1);
        std::cout << "token:" << token1 << " token2:" << token2 << " token3:" << token3 << std::endl;
        if(token1 == "push")
        {
            _command.type = Command::C_PUSH;
        }
        else if(token1 == "pop")
        {
            _command.type = Command::C_POP;
        }
        else if(token1 == "function")
        {
            _command.type = Command::C_FUNCTION;
        }
        else if (token1 == "call")
        {
            _command.type = Command::C_CALL;
        }
        _command.cmd = token1;
        _command.arg1 = token2;
        _command.arg2 = std::stoi(token3);
    }
    else if((tmpStr.find("if-goto")!=-1) ||
            (tmpStr.find("label")!=-1) ||
            (tmpStr.find("goto")!=-1))
    {
        auto index1 = tmpStr.find_first_of(" ");
        token1 = tmpStr.substr(0, index1);
        std::size_t idx = tmpStr.find(" ", index1+1);
        if(idx != -1)
        {
            idx = idx - (index1 + 1);
        }
        else
        {
            idx = tmpStr.size() - (index1 + 1);
        }
        // token2 = tmpStr.substr(index1 + 1, tmpStr.size() - (index1 + 1));
        token2 = tmpStr.substr(index1 + 1, idx);

        if(token1 == "label")
        {
            _command.type = Command::C_LABEL;
        }
        else if(token1 == "if-goto")
        {
            _command.type = Command::C_IF;
        }
        else
        {
            _command.type = Command::C_GOTO;
        }
        _command.cmd = token1;
        _command.arg1 = token2;
        std::cout << "token1:" << token1 << " token2:" << token2 << std::endl;
    }
    else
    {
        //token1 = tmpStr.substr(0, tmpStr.size()-1); // -1
        token1 = tmpStr;
        std::size_t idx = tmpStr.find(" ");
        if(idx != -1)
        {
            token1 = token1.substr(0, idx);
        }
        if(token1 == "return")
        {
            _command.type = Command::C_RETURN;
        }
        else
        {
            _command.type = Command::C_ARITHMETIC;
        }
        _command.cmd = token1;

        std::cout << "token:" << token1 << std::endl;
    }
    std::cout << "------------" << std::endl;
    std::cout << "token:" << token1 << " token2:" << token2 << std::endl;
    std::cout << "------------" << std::endl;
}

Parser::Command::CMDType Parser::commandType()
{
    return _command.type;
}

std::string Parser::arg1()
{
    if(_command.type == Command::CMDType::C_RETURN)
    {
        return "";
    }
    else if(_command.type == Command::CMDType::C_ARITHMETIC)
    {
        return _command.cmd;
    }
    return _command.arg1;
}

int Parser::arg2()
{
    if(_command.type == Command::CMDType::C_PUSH ||
        _command.type == Command::CMDType::C_POP ||
        _command.type == Command::CMDType::C_FUNCTION ||
        _command.type == Command::CMDType::C_CALL)
    {
        return _command.arg2;
    }
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
