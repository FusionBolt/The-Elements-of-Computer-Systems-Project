#include <iostream>
#include <vector>
#include "Parser.h"
#include "CodeWriter.h"

#include <dirent.h>
#include <sys/stat.h>
/* Show all files under dir_name , do not show directories ! */
template<typename Filter>
std::vector<std::string> GetDicFileByFilter(const std::string& dirName, Filter&& filter)
{
    // check the parameter !
    if (dirName.empty())
    {
        std::cout << " dirName is null ! " << std::endl;
        return {};
    }

    // check if dirName is a valid dir
    struct stat s;
    lstat(dirName.c_str(), &s);
    if (!S_ISDIR(s.st_mode))
    {
        std::cout << "dirName is not a valid directory !" << std::endl;
        return {};
    }

    struct dirent *filename;    // return value for readdir()
    DIR *dir;                   // return value for opendir()
    dir = opendir(dirName.c_str());
    if (dir == nullptr)
    {
        std::cout << "Can not open dir " << dirName << std::endl;
        return {};
    }
    std::cout << "Successfully opened the dir !" << std::endl;

    std::vector<std::string> files;
    /* read all the files in the dir ~ */
    while ((filename = readdir(dir)) != nullptr)
    {
        // get rid of "." and ".."
        if (strcmp(filename->d_name, ".") == 0 ||
            strcmp(filename->d_name, "..") == 0)
            continue;
        // std::cout << filename->d_name << std::endl;
        if(filter(filename->d_name))
        {
            files.emplace_back(filename->d_name);
        }
    }
    for(auto& v: files)
    {
        v = dirName + v;
    }
    return files;
}

int main()
{
    std::string path = "/Users/fusionbolt/Desktop/nand2tetris/projects/08/FunctionCalls/FibonacciElement/";
    auto v = GetDicFileByFilter(path,
                                [](const std::string &s) {
                                    // std::cout << s.substr(s.size() -3) << std::endl;
                                    if (s.substr(s.size() - 3) == ".vm")
                                    { return true; }
                                    else
                                    { return false; }
                                });
    for (auto &val:v)
    {
        Parser p(val);
        CodeWriter c(val);
        for (int i = 0; i < 80; ++i)
        {
            p.advance();
            if (p._command.cmd.empty())
            {
                continue;
            }
            switch (p.commandType())
            {
                case Parser::Command::C_PUSH: // ok
                case Parser::Command::C_POP:
                    c.WritePushPop(p._command, p.arg1(), p.arg2());
                    break;
                case Parser::Command::C_ARITHMETIC: // ok
                    c.WriteArithmetic(p._command.cmd);
                    break;
                case Parser::Command::C_FUNCTION: // ok
                    c.WriteFunction(p.arg1(), p.arg2());
                    break;
                case Parser::Command::C_CALL:
                    c.WriteCall(p.arg1(), p.arg2());
                    break;
                case Parser::Command::C_GOTO: // ok
                    c.WriteGoTo(p.arg1());
                    break;
                case Parser::Command::C_LABEL: // ok
                    c.WriteLabel(p.arg1());
                    break;
                case Parser::Command::C_IF: // ok
                    c.WriteIf(p.arg1());
                    break;
                case Parser::Command::C_RETURN: // ok
                    c.WriteReturn();
                    break;
            }
        }
    }
    return 0;
}