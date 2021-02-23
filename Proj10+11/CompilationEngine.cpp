//
// Created by fusionbolt on 2019/9/18.
//
#include <iostream>
#include <map>

#include "CompilationEngine.h"
#include "JackTokenizer.h"

const std::string TrueLabel = "IF_TRUE";
const std::string FalseLabel = "IF_FALSE";
const std::string EndLabel = "IF_END";
const std::string WhileStartLabel = "WHILE_EXP";
const std::string WhileEndLabel = "WHILE_END";

CompilationEngine::CompilationEngine(const std::string& srcFilePath,
        const std::string& xmlFilePath):
        _writer(xmlFilePath), _ifIndex(0)
{
    //std::string fileName = "Main.jack";
    std::cout << "Process file name:" << srcFilePath << std::endl;
    auto firstPos = srcFilePath.find_last_of("/") + 1;
    if(firstPos == std::string::npos)
    {
        firstPos = 0;
    }
    auto lastPos = srcFilePath.find_last_of(".");
    _nowClassName = srcFilePath.substr(firstPos, lastPos - firstPos);
    //std::cout << _nowClassName << std::endl;
    _vmWriter.SetOutPutFileName(_nowClassName + ".vm");
    InitTokens(srcFilePath);
    InitMap();
}

void CompilationEngine::CompileClass()
{
    WriteXMLLeftTag("class");

    Match("class");
    IdentifierMatch();
    Match("{");
    CompileClassVarDec();
//    _subSymbolTable.StartSubRoutine();
    CompileSubroutine();

    Match("}");

    WriteXMLRightTag("class");
}

void CompilationEngine::CompileClassVarDec()
{
//    WriteXMLLeftTag("classVarDec");

    auto token = _nowToken->val;
    auto kind = token;
    if(token == "static" || token == "field")
    {
        WriteXMLLeftTag("classVarDec");
        WriteXMLNode(_nowToken->type, token);
        GetNextToken();
    }
    else
    {
        return;
        // WhenError("Modifier Error");
    }
    auto type = _nowToken->val;
    TypeMatch();
    auto name = _nowToken->val;
    IdentifierMatch();
    _classSymbolTable.Define(name, type, kind);

    while (OptionalMatch(","))
    {
        name = _nowToken->val;
        IdentifierMatch();
        _classSymbolTable.Define(name, type, kind);
    }

    Match(";");

    WriteXMLRightTag("classVarDec");

    CompileClassVarDec();
}

void CompilationEngine::CompileSubroutine()
{
    _subSymbolTable.StartSubRoutine();

    bool isConstructor, isMethod;

    auto token = _nowToken->val;
    if(token == "constructor" || token == "function" || token == "method")
    {
        WriteXMLLeftTag("subroutineDec");
        WriteXMLNode(_nowToken->type, token);
        isConstructor = (token == "constructor");
        isMethod = (token == "method");
        GetNextToken();
    }
    else
    {
        return;
        //WhenError("Subroutine type Error");
    }
    TypeMatch();
    auto functionName = _nowClassName + "." + _nowToken->val;
    IdentifierMatch();
    Match("(");
    CompileParameterList(isMethod);
    Match(")");
    CompileSubroutineBody(functionName, isConstructor, isMethod);

    WriteXMLRightTag("subroutineDec");

    CompileSubroutine();
}

void CompilationEngine::CompileSubroutineBody(const std::string &functionName, bool isConstructor, bool isMethod)
{
    WriteXMLLeftTag("subroutineBody");
    Match("{");
    std::size_t varCount = 0;
    while(_nowToken->val == "var")
    {
        CompileVarDec(varCount);
    }
    _vmWriter.WriteFunction(functionName, varCount);

    SetPointer(isConstructor, isMethod);

    CompileStatements();
    Match("}");

    WriteXMLRightTag("subroutineBody");
}

void CompilationEngine::CompileParameterList(bool isMethod)
{
    WriteXMLLeftTag("parameterList");
    //std::size_t nArgs = 0;
    if(isMethod)
    {
        _subSymbolTable.Define("this", "object", "argument");
    }
    if(_nowToken->val == ")")
    {
        WriteXMLRightTag("parameterList");
        //_vmWriter.WriteFunction(functionName, 0);
        return;
    }
    do
    {
        //++nArgs;
        auto type = _nowToken->val;
        TypeMatch();
        _subSymbolTable.Define(_nowToken->val, type,"argument");
        IdentifierMatch();
    }
    while(OptionalMatch(","));
    //_vmWriter.WriteFunction(functionName, nArgs);

//    for (std::size_t i = 0; i < nArgs; ++i)
//    {
//        _vmWriter.WritePush("argument", i);
//    }

    WriteXMLRightTag("parameterList");
}

void CompilationEngine::CompileVarDec(size_t &varCount)
{
//    WriteXMLLeftTag("varDec");
//    auto kind = _nowToken->val;
//    Match("var"); // kind
//    auto type = _nowToken->val;
//    TypeMatch(); // type
//    auto name = _nowToken->val;
//    IdentifierMatch(); // name
//    _subSymbolTable.Define(name, type, kind);
//    ++varCount;
//    // add
//    while(OptionalMatch(","))
//    {
//        name = _nowToken->val;
//        IdentifierMatch();
//        _subSymbolTable.Define(name, type, kind);
//        // add
//        ++varCount;
//    }
    WriteXMLLeftTag("varDec");
    auto kind = _nowToken->val;
    Match("var"); // kind
    auto type = _nowToken->val;
    TypeMatch(); // type
    do
    {
        auto name = _nowToken->val;
        IdentifierMatch();
        _subSymbolTable.Define(name, type, kind);
        // add
        ++varCount;
    }
    while(OptionalMatch(","));
    Match(";");

    WriteXMLRightTag("varDec");
}

void CompilationEngine::CompileStatements()
{
    WriteXMLLeftTag("statements");

    CompileStatement();

    WriteXMLRightTag("statements");
}

void CompilationEngine::CompileDo()
{
    WriteXMLLeftTag("doStatement");

    Match("do");
    CompileSubroutineCall();
    Match(";");

    _vmWriter.WritePop("temp", 0);
    WriteXMLRightTag("doStatement");
}

void CompilationEngine::CompileLet()
{
    WriteXMLLeftTag("letStatement");

    Match("let");
    auto varName = _nowToken->val;
    IdentifierMatch();

    if(OptionalMatch("["))
    {
        CompileExpression();
        Match("]");

        PushVar(varName);
        _vmWriter.WriteArithmetic("add");

        Match("=");
        CompileExpression();

        _vmWriter.WritePop("temp", 0);
        _vmWriter.WritePop("pointer", 1);
        _vmWriter.WritePush("temp", 0);
        _vmWriter.WritePop("that", 0);
    }
    else
    {
        Match("=");
        CompileExpression();
        // _vmWriter.WritePop("local", _subSymbolTable.IndexOf(varName));
        PopVar(varName);
    }

    Match(";");

    WriteXMLRightTag("letStatement");
}

void CompilationEngine::CompileWhile()
{
    WriteXMLLeftTag("whileStatement");

    auto indexStr = std::to_string(_ifIndex++);
    _vmWriter.WriteLabel(WhileStartLabel + indexStr);

    Match("while");
    Match("(");
    CompileExpression();
    Match(")");
    _vmWriter.WriteArithmetic("not");
    _vmWriter.WriteIf(WhileEndLabel + indexStr);

    Match("{");
    CompileStatements();
    _vmWriter.WriteGoto(WhileStartLabel + indexStr);
    Match("}");
    _vmWriter.WriteLabel(WhileEndLabel + indexStr);

    WriteXMLRightTag("whileStatement");
}

void CompilationEngine::CompileReturn()
{
    WriteXMLLeftTag("returnStatement");

    Match("return");
    if(!OptionalMatch(";"))
    {
        CompileExpression();
        Match(";");
        // return value is result of expression
        // CompileExpression will push result val to the top of stack
    }
    else
    {
        _vmWriter.WritePush("constant", 0);
    }
    _vmWriter.WriteReturn();
    WriteXMLRightTag("returnStatement");
}

void CompilationEngine::CompileIf()
{
    WriteXMLLeftTag("ifStatement");

    Match("if");
    Match("(");
    CompileExpression();
    Match(")");

    auto indexStr = std::to_string(_ifIndex++);

    _vmWriter.WriteIf(TrueLabel + indexStr);
    _vmWriter.WriteGoto(FalseLabel + indexStr); // goto false
    _vmWriter.WriteLabel(TrueLabel + indexStr); // true

    Match("{");
    CompileStatements();
    Match("}");

    _vmWriter.WriteGoto(EndLabel + indexStr);

    _vmWriter.WriteLabel(FalseLabel + indexStr); // false
    if(OptionalMatch("else"))
    {
        Match("{");
        CompileStatements();
        Match("}");
    }
    _vmWriter.WriteLabel(EndLabel + indexStr);
    WriteXMLRightTag("ifStatement");
}

void CompilationEngine::CompileExpression()
{
    WriteXMLLeftTag("expression");

    CompileTerm();
    auto isOp = [](const std::string& symbol)
    {
        std::string op = "+-*/&|<>=";
        return (symbol.find_first_of(op) != std::string::npos);
    };
    auto op = _nowToken->val;
    while(isOp(op))
    {
        Match(op);
        CompileTerm();
        _vmWriter.WriteArithmetic(opMap[op]);
        op = _nowToken->val;
    }

    WriteXMLRightTag("expression");
}

void CompilationEngine::CompileTerm()
{
    WriteXMLLeftTag("term");

    std::string unaryOp = "-~";
    auto tokenType = _nowToken->type;
    auto tokenVal = _nowToken->val;
    if(tokenType == INT_CONST)
    {
        _vmWriter.WritePush("constant", std::stoi(tokenVal));
        WriteXMLNode(tokenType, tokenVal);
        GetNextToken();
    }
    else if(tokenType == STRING_CONST)
    {
        _vmWriter.WritePush("constant", tokenVal.size());
        _vmWriter.WriteCall("String.new", 1);
        for(auto v : tokenVal)
        {
            _vmWriter.WritePush("constant", v);
            _vmWriter.WriteCall("String.appendChar", 2);
        }
        WriteXMLNode(tokenType, tokenVal);
        GetNextToken();
    }
//    else if(tokenType == KEYWORD)
//    {
//        WriteXMLNode(tokenType, tokenVal);
//        GetNextToken();
//    }
    else if(tokenType == IDENTIFIER)
    {
        auto nextToken = (_nowToken + 1)->val;
        if(nextToken == "[")
        {
            IdentifierMatch();
            Match("[");
            CompileExpression();
            Match("]");
            // execute val in []
            PushVar(tokenVal);
            _vmWriter.WriteArithmetic("add");
            // add val in [] and var base address
            _vmWriter.WritePop("pointer", 1);
            _vmWriter.WritePush("that", 0);
            //WriteVar(tokenVal);
        }
        else if(nextToken == "." || nextToken == "(")
        {
            CompileSubroutineCall();
        }
        else
        {
            IdentifierMatch();
            PushVar(tokenVal);
        }
    }
    else
    {
        if(tokenVal == "null" || tokenVal == "false")
        {
            _vmWriter.WritePush("constant", 0);
            GetNextToken();
        }
        else if(tokenVal == "true")
        {
            _vmWriter.WritePush("constant", 1);
            _vmWriter.WriteArithmetic("not");
            GetNextToken();
            //_vmWriter.WritePush("constant", 0);
            //_vmWriter.WriteArithmetic("not");
        }
        else if (tokenVal == "(") // (expression)
        {
            Match("(");
            CompileExpression();
            Match(")");
        }
        else if (tokenVal == "-" || tokenVal == "~") //unaryOp
        {
            WriteXMLNode(tokenType, tokenVal);
            GetNextToken();
            CompileTerm();
            if(tokenVal == "-")
            {
                _vmWriter.WriteArithmetic("neg");
            }
            else if(tokenVal == "~")
            {
                _vmWriter.WriteArithmetic("not");
            }
        }
        else if(tokenVal == "this")
        {
            _vmWriter.WritePush("pointer", 0);
            GetNextToken();
        }
        else
        {
            WhenError("Term Error");
        }
    }

    WriteXMLRightTag("term");
}

void CompilationEngine::CompileExpressionList(const std::string &objName, bool isMethod)
{
    WriteXMLLeftTag("expressionList");

    auto tokenType = _nowToken->type;
    auto tokenVal = _nowToken->val;

    std::size_t count = 0;

    if(isMethod)
    {
        ++count;
    }

    if(tokenVal == ")")
    {
        WriteXMLRightTag("expressionList");
        _vmWriter.WriteCall(objName, count);
        return;
    }

    do
    {
        CompileExpression();
        count++;
    }
    while(OptionalMatch(","));

    _vmWriter.WriteCall(objName, count);
    WriteXMLRightTag("expressionList");

//    if(!(tokenType == INT_CONST || tokenType == STRING_CONST ||
//       tokenType == KEYWORD ||
//       _nowToken->val == "-" || _nowToken->val == "~"))
//    {
//        WriteXMLRightTag("expressionList");
//        _vmWriter.WriteCall(callName, 0);
//        return;
//    }
//    else if(tokenType == IDENTIFIER)
//    {
//        _vmWriter.WritePush();
//    }

}

void CompilationEngine::WhenError(const std::string &errInfo)
{
    std::cout << errInfo << std::endl;
    exit(-1);
}

bool CompilationEngine::IsType(const std::string &s)
{
    return ((s == "int") || (s == "boolean") ||
            (s == "Array") || (s == "char") || (s == "void"));
}

void CompilationEngine::Match(const std::string &symbol)
{
    if(_nowToken->val == symbol)
    {
        WriteXMLNode(_nowToken->type, _nowToken->val);
        GetNextToken();
    }
    else
    {
        WhenError("");
    }
}

void CompilationEngine::IdentifierMatch()
{
    //if(_tokens[_nowTokenIndex].type == identifier)
    if(_nowToken->type == IDENTIFIER)
    {
        WriteXMLNode(_nowToken->type, _nowToken->val);
        GetNextToken();
    }
    else
    {
        WhenError("");
    }
}

void CompilationEngine::TypeMatch()
{
    if(IsType(_nowToken->val) || _nowToken->type == IDENTIFIER)
    {
        WriteXMLNode(_nowToken->type, _nowToken->val);
        GetNextToken();
    }
    else
    {
        WhenError("");
    }
}

bool CompilationEngine::OptionalMatch(const std::string &symbol)
{
    if(_nowToken->val == symbol)
    {
        WriteXMLNode(_nowToken->type, _nowToken->val);
        GetNextToken();
        return true;
    }
    return false;
}

void CompilationEngine::CompileSubroutineCall()
{
    auto subName = _nowToken->val;
    auto classType = GetType(subName);

    IdentifierMatch();
    // subName is method???

    bool isMethod =
            (_subSymbolTable.KindOf(subName) != "null") ||
            (_classSymbolTable.KindOf(subName) != "null");

    if(isMethod)
    {
        PushVar(subName);
    }
    if(OptionalMatch("(")) // call method
    {
        // 直接调用则为method
        isMethod = true;
        _vmWriter.WritePush("pointer", 0); // push self

        CompileExpressionList(_nowClassName + "." + subName, isMethod);
        //CompileExpressionList(subName, isMethod);
        Match(")");
    }
    else if(OptionalMatch("."))
    {
        // 如果前面的标识符未定义则为function，即isMethod

        auto callObj = classType == "null" ? subName : classType;
        auto functionName = _nowToken->val;
        // subName = subName + "." + _nowToken->val;
        subName = callObj + "." + functionName;
        IdentifierMatch();
        Match("(");
        CompileExpressionList(subName, isMethod);
        Match(")");
    }
    else
    {
        WhenError("");
    }
}

void CompilationEngine::CompileStatement()
{
    auto token = _nowToken->val;

    if(token == "let")
    {
        CompileLet();
    }
    else if(token == "if")
    {
        CompileIf();
    }
    else if(token == "while")
    {
        CompileWhile();
    }
    else if(token == "do")
    {
        CompileDo();
    }
    else if(token == "return")
    {
        CompileReturn();
    }
    else
    {
        return;
    }
    CompileStatement();
}

std::string CompilationEngine::GetKind(const std::string& varKind)
{
    if(varKind == "var")
    {
        return "local";
    }
    else if(varKind == "static")
    {
        return "static";
    }
    else if(varKind == "field")
    {
        return "this"; // if field -> main symbol table
        return "local"; // if field -> main symbol table
    }
    else if(varKind == "argument")
    {
        return "argument";
    }
}

std::string CompilationEngine::GetType(const std::string &varName)
{
    auto type = _subSymbolTable.TypeOf(varName);
    if(type == "null")
    {
        type = _classSymbolTable.TypeOf(varName);
    }
    return type;
}

void CompilationEngine::PushVar(const std::string &varName)
{
    auto kind = _subSymbolTable.KindOf(varName);
    if(kind == "null")
    {
        kind = _classSymbolTable.KindOf(varName);
    }
    auto varKind = GetKind(kind);
    if(kind == "field")
    {
        _vmWriter.WritePush(varKind, _classSymbolTable.IndexOf(varName));
    }
    else
    {
        _vmWriter.WritePush(varKind,_subSymbolTable.IndexOf(varName));
    }
}
void CompilationEngine::PopVar(const std::string &varName)
{
    auto kind = _subSymbolTable.KindOf(varName);
    if(kind == "null")
    {
        kind = _classSymbolTable.KindOf(varName);
    }
    auto varKind = GetKind(kind);
    if(kind == "field")
    {
        _vmWriter.WritePop(varKind, _classSymbolTable.IndexOf(varName));
    }
    else
    {
        _vmWriter.WritePop(varKind,_subSymbolTable.IndexOf(varName));
    }
}
void CompilationEngine::InitMap()
{
    typeMap[KEYWORD] = "keyword";
    typeMap[IDENTIFIER] = "identifier";
    typeMap[SYMBOL] = "symbol";
    typeMap[INT_CONST] = "integerConstant";
    typeMap[STRING_CONST] = "stringConstant";

    opMap["+"] = "add";
    opMap["-"] = "sub";
    opMap["&"] = "and";
    opMap["|"] = "or";
    opMap[">"] = "gt";
    opMap["<"] = "lt";
    opMap["="] = "eq";
    opMap["/"] = "call Math.divide 2";
    opMap["*"] = "call Math.multiply 2";
}

void CompilationEngine::InitTokens(const std::string& fileName)
{
    JackTokenizer tokenizer(fileName);

    for (std::size_t i = 0; i < 1000; ++i)
    {
        if(!tokenizer.hasMoreTokens())
        {
            break;
        }
        tokenizer.advance();
        if(tokenizer.tokenType() == JackTokenizer::INT_CONST)
        {
//            _tokens.push_back({std::to_string(tokenizer.intVal()),
//                               tokenizer.tokenType()});
            _tokens.emplace_back(std::to_string(tokenizer.intVal()),
                                 tokenizer.tokenType());
        }
        else
        {
            _tokens.emplace_back(tokenizer.stringVal(), tokenizer.tokenType());
            //_tokens.push_back({tokenizer.stringVal(), tokenizer.tokenType()});
        }
    }
    _nowToken = _tokens.cbegin();
}

void CompilationEngine::SetPointer(bool isConstructor, bool isMethod)
{
    if(isConstructor)
    {
        _vmWriter.WritePush("constant", _classSymbolTable.VarCount("field"));
        _vmWriter.WriteCall("Memory.alloc", 1);
        _vmWriter.WritePop("pointer", 0);
    }
    else if(isMethod)
    {
        _vmWriter.WritePush("argument", 0);
        _vmWriter.WritePop("pointer", 0);
    }
}
