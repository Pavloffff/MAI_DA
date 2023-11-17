#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>

#define TKey int
#define COMPRESS 0
#define DECOMPRESS 1
#define MAX_ALPHA 25
// #define map unordered_map

class ArchiverLZW {
    int type = COMPRESS;
    std::map<TKey, std::string> decompressDict;
    std::map<std::string, TKey> compressDict;
public:
    void Compress(std::string &text);
    void Decompress(std::vector<TKey> &codes);
    void InitDict(int type, int maxAlpha);
    void PrintDict();
    ArchiverLZW(int type, int maxAlpha);
    ArchiverLZW();
    ~ArchiverLZW();
};

void ArchiverLZW::Compress(std::string &text)
{
    if (text.size() == 0) {
        return;
    }
    TKey code = compressDict.size();
    std::string tmpStr;
    for (auto i : text) {
        if (compressDict.find(tmpStr + i) != compressDict.end()) {
            tmpStr += i;
        } else {
            std::cout << compressDict[tmpStr] << " ";
            compressDict[tmpStr + i] = code++;
            tmpStr = i;
        }
    }
    if (!tmpStr.empty()) {
        std::cout << compressDict[tmpStr];
    }
    std::cout << "\n";
}

void ArchiverLZW::Decompress(std::vector<TKey> &codes)
{
    if (codes.empty()) {
        return;
    }
    TKey prev = codes[0], code = decompressDict.size();
    std::cout << decompressDict[codes[0]];
    for (int i = 1; i < codes.size() && codes[i] != 26; i++) {
        if (decompressDict.find(codes[i]) != decompressDict.end()) {
            decompressDict[code++] = decompressDict[prev] + decompressDict[codes[i]][0];
        } else {
            decompressDict[code++] = decompressDict[prev] + decompressDict[prev][0];
        }
        prev = codes[i];
        std::cout << decompressDict[codes[i]];
    }
    std::cout << "\n";
}

void ArchiverLZW::InitDict(int type, int maxAlpha)
{
    if (type == COMPRESS) {
        for (TKey i = 0; i <= maxAlpha; i++) {
            std::string tmpStr(1, (char)('a' + i));
            compressDict[tmpStr] = i;
        }
        std::string tmpStr(1, EOF);
        compressDict[tmpStr] = maxAlpha + 1;
    } else {
        for (TKey i = 0; i <= maxAlpha; i++) {
            std::string tmpStr(1, (char)('a' + i));
            decompressDict[i] = tmpStr;
        }
        std::string tmpStr(1, EOF);
        decompressDict[maxAlpha + 1] = tmpStr;
    }
}

void ArchiverLZW::PrintDict()
{
    if (type == COMPRESS) {
        for (auto i : compressDict) {
            std::cout << i.first << ":\t" << i.second << "\n";
        }
    } else {
        for (auto i : decompressDict) {
            std::cout << i.first << ":\t" << i.second << "\n";
        }
    }
}

ArchiverLZW::ArchiverLZW(int type, int maxAlpha)
{
    this->type = type;
    InitDict(type, maxAlpha);
}

ArchiverLZW::ArchiverLZW()
{
}

ArchiverLZW::~ArchiverLZW()
{
}

int main(int argc, char const *argv[])
{
    std::string command;
    std::cin >> command;
    if (command == "compress") {
        ArchiverLZW tool(COMPRESS, MAX_ALPHA);
        std::string text;
        std::cin >> text;
        std::string tmpStr(1, EOF);
        text += tmpStr;
        tool.Compress(text);
        // tool.PrintDict();
    } else {
        std::vector<TKey> codes;
        TKey code = 0;
        while (std::cin >> code) {
            codes.push_back(code);
        }
        codes.push_back(MAX_ALPHA + 1);
        ArchiverLZW tool(DECOMPRESS, MAX_ALPHA);
        tool.Decompress(codes);
        // tool.PrintDict();
    }
    return 0;
}

