#include "Frequency.h"
#include <iostream>

void Arifm::Frequency::init()
{
    if (cumulative.size()) {
        return;
    }
    unsigned int sum = 0;
    cumulative.push_back(sum);
    for (int i = 0; i < data.size(); i++) {
        sum += data[i];
        cumulative.push_back(sum);
    }
}

unsigned int Arifm::Frequency::get(unsigned int symbol)
{
    return data[symbol];
}

void Arifm::Frequency::set(unsigned int symbol, unsigned int freq)
{
    int tmp = num - data[symbol];
    num += freq;
    data[symbol] = freq;
    cumulative.clear();
}

void Arifm::Frequency::inc(unsigned int symbol)
{
    data[symbol]++;
    num++;
    cumulative.clear();
}

unsigned int Arifm::Frequency::right(unsigned int symbol)
{
    init();
    return cumulative[symbol + 1];
}

unsigned int Arifm::Frequency::left(unsigned int symbol)
{
    init();
    return cumulative[symbol];
}

unsigned int Arifm::Frequency::getNum()
{
    return num;
}

Arifm::Frequency::Frequency()
{
    size = 257;
    data.reserve(size + 1);
    for (int i = 0; i < size; i++) {
        data.push_back(1);
    }
    // std::cout << data.size() << "\n";
    cumulative.reserve(size + 1);
    init();
    num = right(size - 1);
    // std::cout << num << '\n';
}

Arifm::Frequency::~Frequency()
{
}
