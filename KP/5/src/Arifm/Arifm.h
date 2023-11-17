#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>

namespace Arifm
{
    class Compressor
    {
    private:
        bool uncompress = false;
        uint64_t length = 0;
        std::vector<int> frequency;
    public:
        bool Uncompress();
        bool Compress();
        Compressor(std::string &fileName, std::set<std::string> &flags, bool mode);
        Compressor();
        ~Compressor();
    };
};