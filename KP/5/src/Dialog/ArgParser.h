#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <set>

#include <string.h>

#include "Exception.h"

namespace Dialog
{
    class ArgParser
    {
    private:
        int filenameCounter = 0;
        std::set<std::string> defaultFlags = {"-c", "-d", "-k", "-l", "-r", "-t", "-1", "-9", "-h"};
    public:
        std::set<std::string> Parse(int argc, char **argv);
        std::set<std::string> DefaultFlags();
        ArgParser();
        ~ArgParser();
    };
};