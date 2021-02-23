//
// Created by fusionbolt on 2019/9/12.
//

#ifndef PROJ6_SYMBOLTABLE_HPP
#define PROJ6_SYMBOLTABLE_HPP

#include <string>
#include <map>
#include <iostream>

class SymbolTable
{
public:
    SymbolTable()
    {
        // Predefined symbol
        _symbolTable["SP"] = 0;
        _symbolTable["LCL"] = 1;
        _symbolTable["ARG"] = 2;
        _symbolTable["THIS"] = 3;
        _symbolTable["THAT"] = 4;
        for (std::size_t i = 0; i < 16; ++i) {
            _symbolTable["R" + std::to_string(i)] = i;
        }
        _symbolTable["SCREEN"] = 16384; // 0x4000
        _symbolTable["KDB"] = 24576; // 0x6000
    }
    void addEntry(const std::string &symbol, int address)
    {
        // symbol address 加入表
        _symbolTable[symbol] = address;
    }
    bool contains(const std::string &symbol)
    {
        // 是否包含指定symbol
        // return _symbolTable.contains(symbol);
        return (_symbolTable.find(symbol) != _symbolTable.end());
    }
    int GetAddress(const std::string &symbol)
    {
        return _symbolTable[symbol];
        // 返回symbol address
    }

    void print()
    {
        for(auto& i:_symbolTable)
        {
            std::cout << "symbol:" << i.first << " address:" << i.second << std::endl;
        }
    }
private:
    std::map<std::string, int> _symbolTable;
};


#endif //PROJ6_SYMBOLTABLE_HPP
