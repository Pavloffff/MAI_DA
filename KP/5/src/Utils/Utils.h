#pragma once

#include <string>
#include <cstdint>

namespace Utils {
    uint32_t CalculateCRC32(std::string &fileName);
    std::string GenerateRandomString(size_t length);
}