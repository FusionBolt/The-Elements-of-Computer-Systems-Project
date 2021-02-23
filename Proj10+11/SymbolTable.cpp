//
// Created by fusionbolt on 2019/9/24.
//

#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
    KindCountInit();
}

void SymbolTable::StartSubRoutine()
{
    // ??
    _symbolTable.clear();
    KindCountInit();
}

void SymbolTable::Define(const std::string &name, const std::string &type, const std::string& kind)
{
    _symbolTable[name] = Val{type, kind, _kindCount[kind]++};
}

std::size_t SymbolTable::VarCount(const std::string& kind)
{
    return _kindCount[kind];
}

std::string SymbolTable::KindOf(const std::string &name)
{
    if(_symbolTable.find(name) != _symbolTable.end())
    {
        return _symbolTable[name].kind;
    }
    else
    {
        return "null";
    }
}

std::string SymbolTable::TypeOf(const std::string &name)
{
    //return _symbolTable[name].type;
    if(_symbolTable.find(name) != _symbolTable.end())
    {
        return _symbolTable[name].type;
    }
    else
    {
        return "null";
    }
}

std::size_t SymbolTable::IndexOf(const std::string &name)
{
    //return _symbolTable[name].num;
    if(_symbolTable.find(name) != _symbolTable.end())
    {
        return _symbolTable[name].num;
    }
    else
    {
        return -1;
    }
}

void SymbolTable::KindCountInit()
{
    _kindCount["static"] = 0;
    _kindCount["field"] = 0;
    _kindCount["argument"] = 0;
    _kindCount["var"] = 0;
}
