#pragma once
#include <vector>
#include <cstdint>

namespace Arifm
{
    class Frequency
    {
    private:
        std::vector<unsigned int> data;
        std::vector<unsigned int> cumulative;
        unsigned int num;
    public:
        void init();
        uint32_t size;
        unsigned int get(unsigned int symbol);
        void set(unsigned int symbol, unsigned int freq);
        void inc(unsigned int symbol);
        unsigned int left(unsigned int symbol);
        unsigned int right(unsigned int symbol);
        unsigned int getNum();
        Frequency();
        ~Frequency();
    };
};