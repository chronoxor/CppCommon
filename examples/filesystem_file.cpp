/*!
    \file filesystem_file.cpp
    \brief Filesystem file example
    \author Ivan Shynkarenka
    \date 04.09.2016
    \copyright MIT License
*/

#include "filesystem/file.h"
#include "utility/countof.h"

#include <iostream>

int main(int argc, char** argv)
{
    char buffer[] = "The quick brown fox jumps over the lazy dog";

    // Create file for writing
    CppCommon::File file("example.txt");
    file.Create(false, true);

    // Write buffer into the file
    file.Write(buffer, CppCommon::countof(buffer) - 1);

    // Flush the file
    file.Flush();

    // Close the file
    file.Close();

    std::cout << "File size: " << file.size() << std::endl;

    // Read all text from the file
    std::string text = CppCommon::File::ReadAllText(file);

    std::cout << "File content: " << text << std::endl;

    // Remove file
    CppCommon::File::Remove(file);

    return 0;
}
