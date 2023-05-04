#include <iostream>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < 50; i++) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        if (i % 100 == 0) {
            std::cout << "\n";
        }
        if (i % 1000 == 0) {
            for (int i = 0; i < 50; i++) {
                std::cout << i << " ";
            }
        }
        std::cout << (rand() % 50) << " ";
    }
    return 0;
}
