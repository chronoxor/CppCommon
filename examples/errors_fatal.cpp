/*!
    \file errors_fatal.cpp
    \brief Fatal abort execution example
    \author Ivan Shynkarenka
    \date 04.04.2016
    \copyright MIT License
*/

#include "errors/fatal.h"

int main(int argc, char** argv)
{
    fatality("My fatal error!");
    return 0;
}
