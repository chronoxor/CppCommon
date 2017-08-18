/*!
    \file reader.cpp
    \brief Reader interface implementation
    \author Ivan Shynkarenka
    \date 07.12.2016
    \copyright MIT License
*/

#include "common/reader.h"

#include "utility/countof.h"

namespace CppCommon {

std::vector<uint8_t> Reader::ReadAllBytes()
{
    const size_t CHUNK = 8192;

    uint8_t buffer[CHUNK];
    std::vector<uint8_t> result;
    size_t size = 0;

    do
    {
        size = Read(buffer, countof(buffer));
        result.insert(result.end(), buffer, buffer + size);
    } while (size == countof(buffer));

    return result;
}

std::string Reader::ReadAllText()
{
    std::vector<uint8_t> bytes = ReadAllBytes();
    return std::string(bytes.begin(), bytes.end());
}

std::vector<std::string> Reader::ReadAllLines()
{
    std::string temp;
    std::vector<std::string> result;
    std::vector<uint8_t> bytes = ReadAllBytes();

    for (auto ch : bytes)
    {
        if ((ch == '\r') || (ch == '\n'))
        {
            if (!temp.empty())
            {
                result.push_back(temp);
                temp.clear();
            }
        }
        else
            temp += ch;
    }

    return result;
}

} // namespace CppCommon
