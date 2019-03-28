/*!
    \file writer.cpp
    \brief Writer interface implementation
    \author Ivan Shynkarenka
    \date 07.12.2016
    \copyright MIT License
*/

#include "common/writer.h"

#include "system/environment.h"

namespace CppCommon {

size_t Writer::Write(const std::string& text)
{
    return Write(text.data(), text.size());
}

size_t Writer::Write(const std::vector<std::string>& lines)
{
    static std::string endline = Environment::EndLine();

    size_t result = 0;
    for (const auto& line : lines)
    {
        if (Write(line.data(), line.size()) != line.size())
            break;
        if (Write(endline.data(), endline.size()) != endline.size())
            break;
        ++result;
    }
    return result;
}

} // namespace CppCommon
