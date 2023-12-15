#include "Utils.h"

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <zlib.h>

uint64_t Utils::BinPow(uint64_t x, uint64_t y)
{
    uint64_t z = 1;
    while (y > 0) {
        if (y % 2 != 0) {
            z = (z * x);
        }
        x = (x * x);
        y /= 2;
    }
    return z;
}

uint32_t Utils::CalculateCRC32(std::string & fileName)
{
    std::ifstream input(fileName);
    std::vector<uint8_t> buffer(4096);
    uint32_t crc = crc32(0L, Z_NULL, 0);
    while (input) {
        input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        crc = crc32(crc, buffer.data(), input.gcount());
    }
    input.close();
    return crc;
}

std::string Utils::GenerateRandomString(size_t length)
{
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string randomString;
    for (size_t i = 0; i < length; ++i) {
        randomString += characters[distribution(generator)];
    }

    return randomString;
}
