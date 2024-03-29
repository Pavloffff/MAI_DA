#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <limits>
#include "../Utils/BitStream.h"
#include "../Utils/Utils.h"

namespace LZW
{
    class Compressor
    {
    private:
        std::string inputFileName;
        std::string outputFileName;
        uint8_t compressValue;
        bool uncompress = false;
        bool consoleInput = false;
        bool consoleOutput = false;
        std::unordered_map<uint32_t, std::string> uncompressDict;
        std::unordered_map<std::string, uint32_t> compressDict;
        int64_t code;
        uint32_t numBits;
    public:
        void InitDict();
        bool Uncompress();
        bool Compress();
        Compressor(std::string &inputFileName,
                   std::string &outputFileName, 
                   bool uncompress, 
                   bool consoleInput, 
                   bool consoleOutput,
                   uint8_t compressValue);
        Compressor();
        ~Compressor();
    };
};
