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

    if (consoleInput) {
        fileName = Utils::GenerateRandomString(16);
        std::ofstream tmpFile(fileName);
        char c;
        while (std::cin.get(c)) {
            tmpFile.put(c);
        }
        tmpFile.close();
    }

    std::ifstream input(fileName);
    BitStream::IStream bitInput(input);
    std::string outputFileName = fileName.substr(0, fileName.find_last_of("."));
    std::ofstream output(outputFileName);
    std::vector<uint8_t> buffer(4);
    input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

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
            output.write(uncompressDict[prev].c_str(), uncompressDict[prev].size());
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
            output.write(uncompressDict[codesI].c_str(), uncompressDict[codesI].size());
        }
    }
    input.close();
    output.close();
    if (consoleInput) {
        std::filesystem::remove(fileName);
    }
    if (consoleOutput) {
        std::filesystem::remove(outputFileName);
    }

    return true;
}

bool LZW::Compressor::Compress()
{
    if (uncompress) {
        return false;
    }
    if (consoleInput) {
        fileName = Utils::GenerateRandomString(16);
        std::ofstream tmpFile(fileName);
        char c;
        while (std::cin.get(c)) {
            tmpFile.put(c);
        }
        tmpFile.close();
    }

    std::ifstream input(fileName);
    std::ofstream output;
    output = std::ofstream(fileName + ".slzw");
    uint32_t checksum = Utils::CalculateCRC32(fileName);
    output.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));
    BitStream::OStream bitOutput(consoleOutput ? std::cout : output);
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
            if (numBits == 16) { // TODO тут число поменять на переменную
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

    if (consoleInput) {
        std::filesystem::remove(fileName);
    }
    if (consoleOutput) {
        std::filesystem::remove(fileName + ".slzw");
    }

    return true;
}

LZW::Compressor::Compressor(std::string &fileName, 
                            bool uncompress, 
                            bool consoleInput, 
                            bool consoleOutput)
{
    this->uncompress = uncompress;
    this->consoleInput = consoleInput;
    this->consoleOutput = consoleOutput;
    this->fileName = fileName;
    InitDict();
}

LZW::Compressor::Compressor()
{
}

LZW::Compressor::~Compressor()
{
}
