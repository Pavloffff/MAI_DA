#include <iostream>
#include <vector>
#include <queue>

class Word
{
public:
    uint32_t wordData;
    uint32_t strIndex;
    uint32_t wordIndex;
    bool operator==(const uint32_t &s) const;
    Word();
    Word(uint32_t v, uint32_t si, uint32_t wi);
    ~Word();
};

Word::Word()
{
}

Word::Word(uint32_t v, uint32_t si, uint32_t wi)
{
    wordData = v;
    strIndex = si;
    wordIndex = wi;
}

bool Word::operator==(const uint32_t &s) const
{
    return wordData == s;
}

Word::~Word()
{
}


int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    uint32_t strIter = 0, wordIter = 0;
    uint32_t wordData = 0;
    std::vector<Word> pattern;
    bool flag = false;
    char c;
    while ((c = getchar()) != '\n') {
        if (!((c < '0') | (c > '9'))) {
            wordData = (wordData << 3) + (wordData << 1) + (c ^ 48);
            flag = true;
        } else if (flag) {
            Word word(wordData, strIter, wordIter);
            pattern.push_back(word);
            wordData = 0;
            flag = 0;
        }
    }
    if (flag) {
        Word word(wordData, strIter, wordIter);
        pattern.push_back(word);
    }
    int sizeOfPattern = pattern.size();
    Word w(-1, 0, 0);
    pattern.push_back(w);
    
    flag = 0;
    wordData = 0;
    wordIter = 1;
    strIter = 1;
    while ((c = getchar()) != EOF) {
        if (!((c < '0') | (c > '9'))) {
            wordData = (wordData << 3) + (wordData << 1) + (c ^ 48);
            flag = true;
        } else if (flag) {
            Word word(wordData, strIter, wordIter);
            pattern.push_back(word);
            wordIter++;
            flag = 0;
            wordData = 0;
        }
        if (c == '\n') {
            strIter++;
            wordIter = 1;
        }
    }
    if (flag) {
        Word word(wordData, strIter, wordIter);
        pattern.push_back(word);
    }
    int sz = pattern.size();
    std::vector<int> ZFunc(sz);
    for (int i = 0; i < sz; i++) {
        ZFunc[i] = 0;
    }
    if (sz < 2) {
        if (sz == 1) {
            ZFunc[0] = sz;
        }
    } else {
        ZFunc[0] = sz;
        int l = 0, r = 0;
        for (int i = 0; i < sz; i++) {
            ZFunc[i] = std::max(0, std::min(r - i, ZFunc[i - l]));
            while (i + ZFunc[i] < sz && pattern[ZFunc[i]].wordData == pattern[i + ZFunc[i]].wordData) {
                ZFunc[i]++;
            }
            if (i + ZFunc[i] > r) {
                l = i;
                r = i + ZFunc[i];
            }
        }
        for (int i = 0; i < sz; i++) {
            if (ZFunc[i] == sizeOfPattern) {
                printf("%d, %d\n", pattern[i].strIndex, pattern[i].wordIndex);
            }
        }
    }
}