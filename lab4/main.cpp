#include <iostream>
#include <vector>
#include <queue>

int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    // scanf pattern
    uint32_t wordData = 0;
    std::vector<int32_t> pattern;
    bool flag = false;
    char c;
    while ((c = getchar()) != '\n') {
        if (!((c < '0') | (c > '9'))) {
            wordData = (wordData << 3) + (wordData << 1) + (c ^ 48);
            flag = true;
        } else if (flag) {
            pattern.push_back(wordData);
            wordData = 0;
            flag = 0;
        }
    }
    if (flag) {
        pattern.push_back(wordData);
    }

    // Z-function for pattern
    uint32_t sz = pattern.size();
    std::vector<uint32_t> Z(sz);
    if (sz < 2) {
        if (sz == 1) {
            Z[0] = sz;
        }
    } else {
        Z[0] = sz;
        uint32_t l = 0, r = 0;
        for (uint32_t i = 0; i < sz; i++) {
            Z[i] = std::max((uint32_t) 0, std::min(r - i, Z[i - l]));
            while (i + Z[i] < sz && pattern[Z[i]] == pattern[i + Z[i]]) {
                Z[i]++;
            }
            if (i + Z[i] > r) {
                l = i;
                r = i + Z[i];
            }
        }
    }

    // spi-function for pattern
    std::vector<uint32_t> spi(sz);
    for (uint32_t i = sz - 1; i > 0; i--) {
        if (Z[i] != 0) {
            spi[i + Z[i] - 1] = Z[i];
        }
    }

    // KMP algorithm
    std::queue<std::pair<int32_t, int32_t>> q;
    uint32_t pIter = 0;
    wordData = 0;
    flag = 0;
    uint32_t strIter = 1, wordIter = 1;
    while ((c = getchar()) != EOF) {
        if (!((c < '0') | (c > '9'))) {
            wordData = (wordData << 3) + (wordData << 1) + (c ^ 48);
            flag = true;
        } else if (flag) {
            while (pIter > 0 && wordData != pattern[pIter]) {
                pIter = spi[pIter - 1];
            }
            if (q.size() == sz) {
                q.pop();
            }
            q.push(std::make_pair(strIter, wordIter));
            if (wordData == pattern[pIter]) {
                pIter++;
            }
            if (pIter == sz) {
                std::cout << q.front().first << ", " << q.front().second << "\n";
                pIter = spi[pIter - 1];
            }
            
            wordIter++;
            wordData = 0;
            flag = 0;
            // std::cout << pIter << "\n"; 
        }
        if (c == '\n') {
            strIter++;
            wordIter = 1;
        }
    }
    if (flag) {
        while (pIter > 0 && wordData != pattern[pIter]) {
            pIter = spi[pIter - 1];
        }
        if (q.size() == sz) {
            q.pop();
        }
        q.push(std::make_pair(strIter, wordIter));
        if (wordData == pattern[pIter]) {
            pIter++;
        }
        if (pIter == sz) {
            // std::cout << strIter << ", " << wordIter << "\n";
            std::cout << q.front().first << ", " << q.front().second << "\n";
            pIter = spi[pIter - 1];
        }
        
        wordIter++;
        wordData = 0;
        flag = 0;
    }
    return 0;
}