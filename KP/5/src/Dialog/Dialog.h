#pragma once

#include <vector>
#include <iomanip>
#include <filesystem>

#include "ArgParser.h"
#include "../Arifm/Arifm.h"
#include "../Lzw/Lzw.h"


namespace Dialog
{
    class Archiver
    {
    private:
        std::string extension = "slzw";
        std::string progName;
        bool keep = false;
        bool check = false;
        bool lFirst = true;
        std::set<std::string> defaultFlags;
        std::set<std::string> argSet;
    public:
        bool TryFlags(std::set<std::string> &flags, std::string &fileName);
        void Run();
        void Help();
        Archiver(int argc, char **argv);
        Archiver();
        ~Archiver();
    };
};