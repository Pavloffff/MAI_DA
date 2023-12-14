#include "Arifm.h"

uint32_t Arifm::Compressor::CalculateCRC32(std::string &fileName)
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

bool Arifm::Compressor::Uncompress()
{
    if (!uncompress) {
        return false;
    }
    std::ifstream input(fileName);
    uint32_t firstChecksum;
    input.read(reinterpret_cast<char*>(&firstChecksum), sizeof(firstChecksum));
    
    BitStream::IStream bitInput(input);
    std::string outputFileName = fileName.substr(0, fileName.find_last_of("."));
    std::ofstream output(outputFileName);
    Frequency freqs;

    for (int i = 0; i < sumRange; i++) {
        int bit = bitInput.read();
        if (bit == -1) {
            bit = 0;
        }
        code = code << 1 | bit;
    }
    while (1) {
        uint32_t maxFreq = freqs.getNum();
        uint64_t range = right - left + 1;
        uint64_t shift = code - left;
        uint64_t target = ((shift + 1) * maxFreq - 1) / range;
        uint32_t l = 0;
        uint32_t r = 257;
        while (r - l > 1) {
            uint32_t mid = (r + l) / 2;
            if (freqs.left(mid) > target) {
                r = mid;
            } else {
                l = mid;
            }
        }
        uint32_t leftSymbol = freqs.left(l);
        uint32_t rightSymbol = freqs.right(l);
        uint64_t newLeft = left + leftSymbol * range / maxFreq;
        uint64_t newRight = left + rightSymbol * range / maxFreq - 1;
        left = newLeft;
        right = newRight;
        while (((left ^ right) & half) == 0) {
            int bit = bitInput.read();
            if (bit == -1) {
                bit = 0;
            }
            code = ((code << 1) & mask) | bit;
            left = ((left << 1) & mask);
            right = ((right << 1) & mask) | 1;
        }
        while ((left & ~right & quarter) != 0) {
            int bit = bitInput.read();
            if (bit == -1) {
                bit = 0;
            }
            code = (code & half) | ((code << 1) & (mask >> 1)) | bit;
            left = (left << 1) ^ half;
            right = ((right ^ half) << 1) | half | 1;
        }
        if (l == 256) {
            break;
        }
        int byte = static_cast<int>(l);
        if (std::numeric_limits<char>::is_signed) {
            byte -= (byte >> 7) << 8;
        }
        output.put(static_cast<char>(byte));
        freqs.inc(l);
    }
    input.close();
    output.close();
    
    uint32_t secondChecksum = CalculateCRC32(outputFileName);

    return firstChecksum == secondChecksum;
}

bool Arifm::Compressor::Compress()
{
    if (uncompress) {
        return false;
    }
    std::ifstream input(fileName);
    std::ofstream output(fileName + ".slzw");
    uint32_t checksum = CalculateCRC32(fileName);
    output.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));
    BitStream::OStream bitOutput(output);
    Frequency freqs;
    while (1) {
        int c = input.get();
        if (c == std::char_traits<char>::eof()) {
		    break;
        }
        uint32_t symbol = static_cast<uint32_t>(c);
        uint64_t range = right - left + 1;
        uint32_t maxFreq = freqs.getNum();
        uint32_t leftSymbol = freqs.left(symbol);
        uint32_t rightSymbol = freqs.right(symbol);
        uint64_t newLeft = left + leftSymbol * range / maxFreq;
        uint64_t newRight = left + rightSymbol * range / maxFreq - 1;
        left = newLeft;
        right = newRight;
        while (((left ^ right) & half) == 0) {
            int bit = static_cast<int>(left >> (sumRange - 1));
            bitOutput.write(bit);
            for (; leftBits > 0; leftBits--) {
		        bitOutput.write(bit ^ 1);
            }
            left = ((left << 1) & mask);
            right = ((right << 1) & mask) | 1;
        }
        while ((left & ~right & quarter) != 0) {
            leftBits++;
            left = (left << 1) ^ half;
            right = ((right ^ half) << 1) | half | 1;
        }
        freqs.inc(symbol);
    }
    uint64_t range = right - left + 1;
    uint32_t maxFreq = freqs.getNum();
    uint32_t leftSymbol = freqs.left(256);
    uint32_t rightSymbol = freqs.right(256);
    uint64_t newLeft = left + leftSymbol * range / maxFreq;
    uint64_t newRight = left + rightSymbol * range / maxFreq - 1;
    left = newLeft;
    right = newRight;
    while (((left ^ right) & half) == 0) {
        int bit = static_cast<int>(left >> (sumRange - 1));
        bitOutput.write(bit);
        for (; leftBits > 0; leftBits--) {
	        bitOutput.write(bit ^ 1);
        }
        left = ((left << 1) & mask);
        right = ((right << 1) & mask) | 1;
    }
    while ((left & ~right & quarter) != 0) {
        leftBits++;
        left = (left << 1) ^ half;
        right = ((right ^ half) << 1) | half | 1;
    }
    freqs.inc(256);
    bitOutput.write(1);
    bitOutput.fillZeros();
    input.close();
    output.close(); 
    return true;
}

Arifm::Compressor::Compressor(std::string &fileName, bool uncompress)
{
    this->uncompress = uncompress;
    this->fileName = fileName;
    sumRange = 32;
    max = 1ULL << sumRange;
    half = max >> 1;
    quarter = half >> 1;
    min = quarter + 2;
    leftBits = 0;
    mask = max - 1;
    left = 0;
    right = mask;
    code = 0;
}

Arifm::Compressor::Compressor()
{
}

Arifm::Compressor::~Compressor()
{
}
