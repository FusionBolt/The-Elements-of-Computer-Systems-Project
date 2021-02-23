//
// Created by fusionbolt on 2019/9/13.
//

#ifndef PROJ6_UTILS_H
#define PROJ6_UTILS_H

#include <string>
bool IsSymbol(const std::string& str)
{
    return !(str[0] >= '0' && str[0] <= '9');
}
#endif //PROJ6_UTILS_H
