#include "BitStream.h"
#include <string>
#include <limits>

BitStream::IStream::IStream(std::istream &stream):
    stream(stream),
    cur(0),
    leftBits(0)
{
}

int BitStream::IStream::read()
{
    if (cur == std::char_traits<char>::eof()) {
        return -1;
    }
    if (leftBits == 0) {
        cur = stream.get();
        if (cur == std::char_traits<char>::eof()) {
            return -1;
        }
        leftBits = 8;
    }
    leftBits--;
    return (cur >> leftBits) & 1;
}

BitStream::OStream::OStream(std::ostream &stream):
    stream(stream),
    cur(0),
    leftBits(0)
{
}

void BitStream::OStream::write(int bit)
{
    cur = (cur << 1) | bit;
    leftBits++;
    if (leftBits == 8) {
        if (std::numeric_limits<char>::is_signed) {
            cur -= (cur >> 7) << 8;
        }
        stream.put(static_cast<char>(cur));
        cur = 0;
        leftBits = 0;
    }
}

void BitStream::OStream::fillZeros()
{
    while (leftBits != 0) {
        write(0);
    }
}
