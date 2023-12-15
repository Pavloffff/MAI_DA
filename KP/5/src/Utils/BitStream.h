#pragma once

#include <iostream>

namespace BitStream
{
    class IStream
    {
    private:
        std::istream &stream;
        int cur;
        int leftBits;
    public:
        IStream(std::istream &stream);
        int read();
    };

    class OStream
    {
    private:
        std::ostream &stream;
        int cur;
        int leftBits;
    public:
        OStream(std::ostream &stream);
        void write(int bit);
        void fillZeros();
        void fillOnes();
    };
};
