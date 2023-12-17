#include "Lzw.h"
#include <filesystem>
#include <unistd.h>
#include <vector>

void LZW::Compressor::InitDict()
{
    if (uncompress) {
        for (int i = 0; i < 256; i++) {
            std::string tmpStr(1, (char)(i));
            this->uncompressDict[i] = tmpStr;
        }
    } else {
        for (int i = 0; i < 256; i++) {
            std::string tmpStr(1, (char)(i));
            this->compressDict[tmpStr] = i;
        }
    }
    code = 258;
    numBits = 9;
}

bool LZW::Compressor::Uncompress()
{
    if (!uncompress) {
        return false;
    }

    std::ifstream input(inputFileName);
    BitStream::IStream bitInput(input);
    std::ofstream output(outputFileName);

    input.read(reinterpret_cast<char*>(&compressValue), sizeof(compressValue));
    uint32_t firstChecksum;
    input.read(reinterpret_cast<char*>(&firstChecksum), sizeof(firstChecksum));
    
    int prev = 0;
    for (int i = 0; i < numBits; i++) {
        int bit = bitInput.read();
        if (bit == -1) {
            bit = 0;
        }
        prev = prev << 1 | bit;
    }
    output.write(uncompressDict[prev].c_str(), uncompressDict[prev].size());
    while (input.good()) {
        int codesI = 0;
        for (int i = 0; i < numBits; i++) {
            int bit = bitInput.read();
            if (bit == -1) {
                bit = 0;
            }
            codesI = codesI << 1 | bit;
        }
        if (codesI == 256) {
            uncompressDict.clear();
            InitDict();
            prev = 0;
            for (int i = 0; i < numBits; i++) {
                int bit = bitInput.read();
                if (bit == -1) {
                    bit = 0;
                }
                prev = prev << 1 | bit;
            }
            if (prev == 257) {
                break;
            }
            if (consoleOutput) {
                std::cout << uncompressDict[prev];
            } else {
                output.write(uncompressDict[prev].c_str(), uncompressDict[prev].size());
            }
        } else {
            if (uncompressDict.find(codesI) != uncompressDict.end()) {
                uncompressDict[code++] = uncompressDict[prev] + uncompressDict[codesI][0];
            } else {
                uncompressDict[code++] = uncompressDict[prev] + uncompressDict[prev][0];
            }
            prev = codesI;
            if (code + 1 == Utils::BinPow(2, numBits)) {
                numBits++;
            }
            if (codesI == 257) {
                break;
            }
            if (consoleOutput) {
                std::cout << uncompressDict[codesI];
            } else {
                output.write(uncompressDict[codesI].c_str(), uncompressDict[codesI].size());
            }
        }
    }
    input.close();
    output.close();
    
    uint32_t secondCheckSum = Utils::CalculateCRC32(outputFileName);
    return firstChecksum == secondCheckSum;
}

bool LZW::Compressor::Compress()
{
    if (uncompress) {
        return false;
    }
    if (consoleInput) {
        std::ofstream tmpFile(inputFileName);
        char c;
        while (std::cin.get(c)) {
            tmpFile.put(c);
        }
        tmpFile.close();
    }
    std::ifstream input(inputFileName);
    std::ofstream output(outputFileName);

    output.write(reinterpret_cast<const char*>(&compressValue), sizeof(compressValue));
    uint32_t checksum = Utils::CalculateCRC32(inputFileName);
    output.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));
    
    BitStream::OStream bitOutput(output);
    std::string tmpStr;
    while (1) {
        int c = input.get();
        if (c == std::char_traits<char>::eof()) {
		    break;
        }
        char symbol = static_cast<char>(c);
        if (compressDict.find(tmpStr + symbol) != compressDict.end()) {
            tmpStr += symbol;
        } else {
            for (int i = numBits - 1; i >= 0; i--) {
                bitOutput.write((compressDict[tmpStr] >> i) & 1);
            }
            compressDict[tmpStr + symbol] = code++;
            if (code == Utils::BinPow(2, numBits)) {
                numBits++;
            }
            if (numBits == compressValue) {
                uint32_t cls = 256;
                for (int i = numBits - 1; i >= 0; i--) {
                    bitOutput.write((cls >> i) & 1);
                }
                compressDict.clear();
                InitDict();
                numBits = 9;
            }
            tmpStr = symbol;
        }
    }
    if (!tmpStr.empty()) {
        for (int i = numBits - 1; i >= 0; i--) {
            bitOutput.write((compressDict[tmpStr] >> i) & 1);
        }
    }
    uint32_t eof = 257;
    for (int i = numBits - 1; i >= 0; i--) {
        bitOutput.write((eof >> i) & 1);
    }
    bitOutput.fillZeros();
    input.close();
    output.close();

    return true;
}

LZW::Compressor::Compressor(std::string &inputFileName,
                            std::string &outputFileName,
                            bool uncompress, 
                            bool consoleInput, 
                            bool consoleOutput,
                            uint8_t compressValue)
{
    this->uncompress = uncompress;
    this->consoleInput = consoleInput;
    this->consoleOutput = consoleOutput;
    this->inputFileName = inputFileName;
    this->outputFileName = outputFileName;
    this->compressValue = compressValue;
    InitDict();
}

LZW::Compressor::Compressor()
{
}

LZW::Compressor::~Compressor()
{
}
