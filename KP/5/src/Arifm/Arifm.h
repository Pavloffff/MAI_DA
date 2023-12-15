#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <limits>
#include "Frequency.h"
#include "../Utils/BitStream.h"
#include "../Utils/Utils.h"

namespace Arifm
{
    class Compressor
    {
    private:
        std::string fileName;
        bool uncompress = false;
        bool consoleInput = false;
        bool consoleOutput = false;
        uint64_t length = 0;
        std::vector<int> frequency;
        uint64_t max;
        uint64_t min;
        uint64_t half;
        uint64_t quarter;
        int sumRange;
        uint64_t leftBits;
        uint64_t mask;
        uint64_t left;
        uint64_t right;
        uint64_t code;
    public:
        bool Uncompress();
        bool Compress();
        Compressor(std::string &fileName, 
                   bool uncompress, 
                   bool consoleInput, 
                   bool consoleOutput);
        Compressor();
        ~Compressor();
    };
};