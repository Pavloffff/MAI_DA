#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <limits>
#include <zlib.h>
#include "Frequency.h"
#include "../Utils/BitStream.h"

namespace Arifm
{
    class Compressor
    {
    private:
        std::string fileName;
        bool uncompress = false;
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
        uint32_t CalculateCRC32(std::string &fileName);
        bool Uncompress();
        bool Compress();
        Compressor(std::string &fileName, bool uncompress);
        Compressor();
        ~Compressor();
    };
};