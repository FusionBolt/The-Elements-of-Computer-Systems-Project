//
// Created by fusionbolt on 2019/9/18.
//

#ifndef PROJ10_JACKTOKENIZER_H
#define PROJ10_JACKTOKENIZER_H

#include <string>
#include <fstream>
#include <variant>
#include <vector>

class JackTokenizer
{
public:
    enum TokenType
    {
        KEYWORD,
        SYMBOL,
        IDENTIFIER,
        INT_CONST,
        STRING_CONST
    };

    struct Token
    {
        TokenType type;
        std::variant<std::string, int16_t> val;
        int line;
    };

    JackTokenizer(const std::string& filePath);

    ~JackTokenizer();

    bool hasMoreTokens();

    void advance();

    TokenType tokenType();

    std::string keyword();

    std::string symbol();

    std::string identifier();

    int16_t intVal();

    std::string stringVal();

private:
    bool IsKeywords(const std::string &symbol);

    bool IsIdentifierChar(char c);

    void InitSymbolAndKeywords();

    std::ifstream _srcFile;

    std::string _srcCode;

    Token _token;

    std::size_t _index = 0;

    std::string _symbols;

    std::vector<std::string> _keywords;

    std::size_t _nowLine;
};


#endif //PROJ10_JACKTOKENIZER_H
