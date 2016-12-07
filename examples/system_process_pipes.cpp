/*!
    \file system_process_pipes.cpp
    \brief Process pipes example
    \author Ivan Shynkarenka
    \date 07.12.2016
    \copyright MIT License
*/

#include "system/environment.h"
#include "system/process.h"

#include <algorithm>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        std::string message = "test message";
        std::string endline = CppCommon::Environment::EndLine();

        std::cout << "Executing child process..." << std::endl;
        CppCommon::Pipe input;
        CppCommon::Pipe output;
        CppCommon::Pipe error;
        CppCommon::Process child = CppCommon::Process::Execute(argv[1], nullptr, nullptr, nullptr, &input, &output, &error);
        input.Write(message);
        input.Write(endline);
        std::string out = output.ReadAllText();
        std::string err = error.ReadAllText();
        int result = child.Wait();
        std::cout << "Executed child process! Result = " << result << std::endl;
        std::cout << "stdout: " << out << std::endl;
        std::cout << "stderr: " << err << std::endl;
        return 0;
    }

    std::string line;
    if (getline(std::cin, line))
    {
        std::cout << line << std::endl;
        std::reverse(line.begin(), line.end());
        std::cerr << line << std::endl;
    }
    return 123;
}
