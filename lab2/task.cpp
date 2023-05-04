#include <iostream>

size_t log2n(int n)
{
    size_t res = 0;
    while (((uint64_t)1 << res) < n) {
        ++res;
    }
    return res;
}

int f(int n)
{
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    int ans = log2n(n + 1);
    return ans;
}

int main(int argc, char const *argv[])
{
    std::cout << f(16) << "\n";
    return 0;
}
