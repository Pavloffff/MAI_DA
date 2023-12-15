#pragma once

#include <string>
#include <cstdint>

namespace Utils 
{
    uint64_t BinPow(uint64_t x, uint64_t y);
    uint32_t CalculateCRC32(std::string &fileName);
    std::string GenerateRandomString(size_t length);
}