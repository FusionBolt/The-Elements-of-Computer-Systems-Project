//
// Created by fusionbolt on 2019/9/24.
//

#ifndef PROJ10_SYMBOLTABLE_H
#define PROJ10_SYMBOLTABLE_H

#include <string>
#include <unordered_map>
#include <list>
#include <map>

class SymbolTable
{
public:
    enum Kind
    {
        NONE,
        STATIC,
        FIELD,
        ARG,
        VAR
    };

    struct Val
    {
        // int / bool / string
        std::string type;
        std::string kind;
        std::size_t num;
    };

    using SymbolTableType = std::map<std::string, Val>;

    SymbolTable();

    void StartSubRoutine();

    void Define(const std::string& name, const std::string& type, const std::string& kind);

    std::size_t VarCount(const std::string& kind);

    std::string KindOf(const std::string& name);

    std::string TypeOf(const std::string& name);

    std::size_t IndexOf(const std::string& name);

    void KindCountInit();

    // std::list<SymbolTableType> _symbolTableList;

    SymbolTableType _symbolTable;

    std::map<std::string, std::size_t> _kindCount;
};


#endif //PROJ10_SYMBOLTABLE_H
