#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[]) 
{
    int n = 0;
    scanf("%d", &n);
    printf("%d\n", n);
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        char letters[256] = {"\0"};
        for (int j = 0; j < 255; j++) {
            char letter1 = rand() % 26 + 97;
            letters[j] = letter1;
        }
        int number = rand() % 100 + 1;
        printf("%s %d\n", letters, number);
    }
    return 0;
}