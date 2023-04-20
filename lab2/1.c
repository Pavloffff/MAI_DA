#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    for (int i = 0; i < 1000000; i++) {
        char letters[256] = {"\0"};
        for (int j = 0; j < 255; j++) {
            char letter1 = rand() % 26 + 97;
            letters[j] = letter1;
        }
        int number = rand() % 100 + 1;
        printf("+ %s %d\n", letters, number);
    }
    return 0;
}