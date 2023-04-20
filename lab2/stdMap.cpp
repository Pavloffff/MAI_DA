#include <iostream>
#include <map>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

void make(std::map<std::string, unsigned long long> &Tree)
{
    std::string key;
    unsigned long long value;
    char c;
    for (int i = 0; i < 250000; i++) {
        std::cin >> c >> key >> value;
        Tree.insert({key, value});
        std::cout << "OK\n";
    }
}

int main(int argc, char const *argv[])
{
    FILE *file;
    if (argc > 1) {
        file = fopen(argv[1], "a");
    }
    clock_t begin, end;
    int function;
    std::cin >> function;
    std::map<std::string, unsigned long long> Tree;
    if (function == 1) {
        begin = clock();
        make(Tree);
    }
    end = clock();
    if (argc > 1) {
        fprintf(file, "%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
        fclose(file);
    }
    return 0;
}
