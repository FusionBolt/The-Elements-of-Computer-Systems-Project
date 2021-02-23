//
// Created by fusionbolt on 2019/9/18.
//

#ifndef PROJ10_COMPILATIONENGINE_H
#define PROJ10_COMPILATIONENGINE_H

#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "SymbolTable.h"
#include "VMWriter.h"

class CompilationEngine
{
public:
    CompilationEngine(const std::string& srcFilePath, const std::string& xmlFilePath);

    void CompileClass();

    void CompileClassVarDec();

    void CompileSubroutine();

    void CompileSubroutineBody(const std::string &functionName, bool isConstructor, bool isMethod);

    void CompileParameterList(bool isMethod);

    void CompileVarDec(size_t &varCount);

    void CompileStatements();

    void CompileStatement();

    void CompileDo();

    void CompileLet();

    void CompileWhile();

    void CompileReturn();

    void CompileIf();

    void CompileExpression();

    void CompileTerm();

    void CompileExpressionList(const std::string &objName, bool isMethod);

    void CompileSubroutineCall();

    enum TokenType
    {
        KEYWORD,
        SYMBOL,
        IDENTIFIER,
        INT_CONST,
        STRING_CONST,
    };

    struct Token{
        Token(const std::string& s, std::size_t nType): val(s), type(nType){}
        std::string val;
        std::size_t type;
    };

private:
    void InitTokens(const std::string& fileName);

    void InitMap();

    void Match(const std::string& symbol);

    void IdentifierMatch();

    void TypeMatch();

    bool OptionalMatch(const std::string& symbol);

    void GetNextToken() {++_nowToken;}

    void WhenError(const std::string& errInfo);

    bool IsType(const std::string& s);

    void SetPointer(bool isConstructor, bool isMethod);

    std::string GetKind(const std::string& varKind);

    std::string GetType(const std::string& varName);

    void PushVar(const std::string& varName);

    void PopVar(const std::string& varName);

    void WriteXMLNode(std::size_t type, const std::string& val)
    {
        auto typeStr = typeMap[type];
        auto tmpStr = "<" + typeStr + "> " + val + " </" + typeStr + ">\n";
        _writer.write(tmpStr.c_str(), tmpStr.size());
        res += tmpStr;
    }

    void WriteXMLLeftTag(const std::string& tag)
    {
        std::string tmpStr = "<" + tag + ">\n";
        _writer.write(tmpStr.c_str(), tmpStr.size());
        res += tmpStr;
    }

    void WriteXMLRightTag(const std::string& tag)
    {
        std::string tmpStr =  "</" + tag + ">\n";
        _writer.write(tmpStr.c_str(), tmpStr.size());
        res += tmpStr;
    }

    std::vector<Token> _tokens;

    std::vector<Token>::const_iterator _nowToken;

    std::map<std::size_t, std::string> typeMap;
    // std::size_t _nowTokenIndex;

    std::map<std::string, std::string> opMap;

    std::ofstream _writer;

    std::string res;

    SymbolTable _classSymbolTable, _subSymbolTable;

    VMWriter _vmWriter;

    std::string _nowClassName;

    std::size_t _ifIndex;
};


#endif //PROJ10_COMPILATIONENGINE_H
