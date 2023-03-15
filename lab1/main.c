#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printString(char *s)
{
    for (int i = 0; s[i] > 0; ++i)
        putc_unlocked(s[i], stdout);
}

// vector of keys

typedef struct
{
    int *buffer;
    int sz;
    int cp;
} VectorInt;

void VectorIntCreate(VectorInt *v)
{
    v->buffer = (int *) malloc(sizeof(int));
    v->sz = 0;
    v->cp = 1;
}

void VectorIntPushBack(VectorInt *v, int value)
{
    if (v->cp <= v->sz) {
        v->cp *= 2;
        v->buffer = (int *) realloc(v->buffer, v->cp * sizeof(int));
    }
    v->buffer[v->sz++] = value;
}

void VectorIntDestroy(VectorInt *v)
{
    free(v->buffer);
    v->buffer = NULL;
}

// vector of values

typedef struct
{
    char **buffer;
    int sz;
    int cp;
} VectorString64;

void VectorString64Create(VectorString64 *v)
{
    v->buffer = (char **) malloc(sizeof(char *));
    v->sz = 0;
    v->cp = 1;
}

void VectorString64PushBack(VectorString64 *v)
{
    if (v->cp - 1 <= v->sz) {
        v->cp *= 2;
        v->buffer = (char **) realloc(v->buffer, v->cp * sizeof(char *));
    }
    v->buffer[v->sz] = (char *) malloc(65 * sizeof(char));
    scanf("%64s", v->buffer[v->sz]);
    ++v->sz;
}

void VectorString64Destroy(VectorString64 *v)
{
    for (int i = 0; i < v->sz; i++) {
        free(v->buffer[i]);
        v->buffer[i] = NULL;
    }
    free(v->buffer);
    v->buffer = NULL;
}

// sort function

void countingSort(VectorInt *keys, int *ansV)
{
    int size = keys->sz;
    if (size <= 1) {
        return;
    }
    int max = 0, min = 1000000;
    for (int i = 0; i < size; ++i) {
        if (keys->buffer[i] > max) {
            max = keys->buffer[i];
        }
        if (keys->buffer[i] < min) {
            min = keys->buffer[i];
        }
    }
    if (max == 0) {
        return;
    }
    int *hash = (int *) malloc((max - min + 1) * sizeof(int));
    memset(hash, 0, (max - min + 1) * sizeof(int));
    for (int i = 0; i < size; ++i) {
        hash[keys->buffer[i] - min]++;
    }
    for (int i = 1; i < max - min + 1; ++i) {
        hash[i] = hash[i] + hash[i - 1];
    }
    for (int i = size - 1; i >= 0; --i) {
        ansV[hash[keys->buffer[i] - min] - 1] = i;
        hash[keys->buffer[i] - min]--;
    }
    free(hash);
}

int main(int argc, char const *argv[])
{
    VectorInt keys;
    VectorIntCreate(&keys);

    VectorString64 values;
    VectorString64Create(&values);

    int key;
    while (scanf("%d", &key) != EOF) {
        VectorIntPushBack(&keys, key);
        getc(stdin);
        VectorString64PushBack(&values);
    }

    int *ansV = (int *) malloc(keys.sz * sizeof(int));
    for (int i = 0; i < keys.sz; ++i) {
        ansV[i] = i;
    }

    countingSort(&keys, ansV);

    for (int i = 0; i < keys.sz; i++) {
        printf("%06d\t%s\n", keys.buffer[ansV[i]], values.buffer[ansV[i]]);
    }

    free(ansV);
    VectorString64Destroy(&values);
    VectorIntDestroy(&keys);
    return 0;
}
