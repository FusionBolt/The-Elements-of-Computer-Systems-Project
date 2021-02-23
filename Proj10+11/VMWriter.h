//
// Created by fusionbolt on 2019/9/24.
//

#ifndef PROJ10_VMWRITER_H
#define PROJ10_VMWRITER_H

#include <string>
#include <fstream>

class VMWriter
{
public:
    VMWriter();

    ~VMWriter();

    void SetOutPutFileName(const std::string& fileName);

    void WritePush(const std::string& seg, std::size_t index);

    void WritePop(const std::string& seg, std::size_t index);

    void WriteArithmetic(const std::string& cmd);

    void WriteLabel(const std::string& label);

    void WriteGoto(const std::string& label);

    void WriteIf(const std::string& label);

    void WriteCall(const std::string& label, std::size_t nArgs);

    void WriteFunction(const std::string& label, std::size_t nArgs);

    void WriteReturn();

    void WriteOutPut();

    std::ofstream _writer;

    std::string outPut;
};


#endif //PROJ10_VMWRITER_H
