#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    int n = atoi(argv[1]);
    srand(time(NULL));
    std::string output;
    for (int i = 0; i < n; i++) {
        char letter = rand() % 26 + 97;
        output += letter;
    }
    std::cout << output << "\n";
    return 0;
}
