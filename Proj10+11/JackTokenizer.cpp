//
// Created by fusionbolt on 2019/9/18.
//

#include <iostream>
#include <sstream>

#include "JackTokenizer.h"

JackTokenizer::JackTokenizer(const std::string& filePath):_srcFile(filePath)
{
    if(!_srcFile.is_open())
    {
        std::cout << "file open failed" << std::endl;
    }

    std::ostringstream tmp;
    tmp << _srcFile.rdbuf();
    _srcCode = tmp.str();

    //std::cout << _srcCode << std::endl;
    // symbol
    InitSymbolAndKeywords();
}

bool JackTokenizer::hasMoreTokens()
{
    if(_index >= _srcCode.size() - 1)
    {
        return false;
    }
    return true;
    // return (!((_srcCode.size() - 1) < _index));
}

void JackTokenizer::advance()
{
    // switch token type
    // token parse
    while(true)
    {
        auto nowChar = _srcCode[_index];
        if(_index >= _srcCode.size() - 1)
        {
            break;
        }
        //std::cout << "r" << std::endl;

        if(nowChar >= '0' && nowChar <= '9')
        {
            // int const
            auto firstIndex = _index;
            while(_srcCode[_index] >= '0' && _srcCode[_index] <= '9')
            {
                _index++;
            }
            _token.val = std::stoi(_srcCode.substr(firstIndex, _index - firstIndex));
            _token.type = INT_CONST;
            break;
        }
        else if((nowChar >= 'a' && nowChar <= 'z') ||
                (nowChar >= 'A' && nowChar <= 'Z'))
        {
            // keyword and identifier
            // auto lastIndex = _srcCode.find(" ", _index + 1); // find
            auto index = _index + 1;
            while(IsIdentifierChar(_srcCode[index]))
            {
                index++;
            }
            auto symbol = _srcCode.substr(_index, index - _index);
            if(IsKeywords(symbol))
            {
                if(symbol == "null")
                {
                    std::cout << "null" << std::endl;
                }
                _token.val = symbol;
                _token.type = KEYWORD;
                std::cout << "token val:" << std::get<std::string>(_token.val) << std::endl;
            }
            else
            {
                _token.val = symbol;
                _token.type = IDENTIFIER;
            }
            _index = index;

            std::cout << std::get<std::string>(_token.val) << std::endl;
            break;
        }
        else if(nowChar == '"')
        {
            // string const
            auto firstIndex = _index;
            auto lastIndex = _srcCode.find('\"', _index + 1);
            _token.val = _srcCode.substr(firstIndex + 1, lastIndex - (firstIndex + 1));
            _token.type = STRING_CONST;
            _index = lastIndex + 1;
            std::cout << "symbol:" << std::get<std::string>(_token.val) << std::endl;
            break;
        }
        else if(nowChar == '\r' || nowChar == '\n' ||
                nowChar == ' ' || nowChar == '\0' || nowChar == '\t')
        {
            // space and linebreak
            _index++;
            continue;
        }
        else if(nowChar == '/')
        {
            // comment
            if(_srcCode[_index + 1] == '/')
            {
                std::string breakLineSymbol = "\r\n";
                // auto lastIndex = _srcCode.find("\r", _index + 2);
                auto lastIndex = _srcCode.find_first_of(breakLineSymbol, _index + 2);
                auto v = _srcCode;
                if(lastIndex == std::string::npos)
                {
                    std::cout << "// can't find";
                }
                _index = lastIndex + 1;
                continue;
            }
            else if(_srcCode[_index + 1] == '*')
            {
                auto lastIndex = _srcCode.find("*/", _index + 2);
                _index = lastIndex + 2;
                continue;
            }
            else
            {
                _token.val = std::string(1, nowChar);
                _token.type = SYMBOL;
                ++_index;
                std::cout << "symbol:" << std::get<std::string>(_token.val) << std::endl;
                break;
            }
        }
        else if(_symbols.find(nowChar) != std::string::npos)
        {
            _token.val = std::string(1, nowChar);
            _token.type = SYMBOL;
            ++_index;
            auto val = std::get<std::string>(_token.val);
            std::cout << "symbol:" << std::get<std::string>(_token.val) << std::endl;
            break;
        }
        else
        {
            auto s = nowChar;
            std::cout << "unspport token:" << s << std::endl;
            ++_index;
        }
    }
}

JackTokenizer::TokenType JackTokenizer::tokenType()
{
    return _token.type;
}

std::string JackTokenizer::keyword()
{
    return std::get<std::string>(_token.val);
}

std::string JackTokenizer::symbol()
{
    return std::get<std::string>(_token.val);
}

std::string JackTokenizer::identifier()
{
    return std::get<std::string>(_token.val);
}

int16_t JackTokenizer::intVal()
{
    return std::get<int16_t >(_token.val);
}

std::string JackTokenizer::stringVal()
{
    if(tokenType() == INT_CONST)
    {
        return "error";
    }
    return std::get<std::string>(_token.val);
}

void JackTokenizer::InitSymbolAndKeywords()
{
    _symbols = "{}()[].,;+-*&|<>=~";
    _keywords.emplace_back("class");
    _keywords.emplace_back("constructor");
    _keywords.emplace_back("function");
    _keywords.emplace_back("method");
    _keywords.emplace_back("field");
    _keywords.emplace_back("static");
    _keywords.emplace_back("var");
    _keywords.emplace_back("int");
    _keywords.emplace_back("char");
    _keywords.emplace_back("boolean");
    _keywords.emplace_back("void");
    _keywords.emplace_back("true");
    _keywords.emplace_back("false");
    _keywords.emplace_back("null");
    _keywords.emplace_back("this");
    _keywords.emplace_back("let");
    _keywords.emplace_back("do");
    _keywords.emplace_back("if");
    _keywords.emplace_back("else");
    _keywords.emplace_back("while");
    _keywords.emplace_back("return");
}

bool JackTokenizer::IsKeywords(const std::string &symbol)
{
    for(auto& v : _keywords)
    {
        if(symbol == v)
        {
            return true;
        }
    }
    return false;
}

bool JackTokenizer::IsIdentifierChar(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') || (c == '_');
}

JackTokenizer::~JackTokenizer()
{
    _srcFile.close();
}
