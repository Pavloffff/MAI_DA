#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>

typedef struct node
{
    char *key;
    uint64_t value;
    int balance;
    struct node *left;
    struct node *right;
    struct node *parent;
} node;

#define NIL &nilNode
node nilNode = {(char *) 0, 0, 0, NIL, NIL, NIL};

node *root = NIL;

node *search(char *k)
{
    node *x = root;
    while (x != NIL) {
        if (strcmp(x->key, k) == 0) {
            return x;
        } else {
            if (strcmp(x->key, k) > 0) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
    }
    return NULL;
}

void leftRotate(node *x)
{
    node *y = x->right;
    x->right = y->left;
    if (y->left != NIL) {
        y->left->parent = x;
    }
    if (y != NIL) {
        y->parent = x->parent;
    }
    if (x->parent == NIL) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    if (x != NIL) {
        x->parent = y;
    }
}

void rightRotate(node *y)
{
    node *x = y->left;
    y->left = x->right;
    if (x->right != NIL) {
        x->right->parent = y;
    }
    if (x != NIL) {
        x->parent = y->parent;
    }
    if (y->parent == NIL) {
        root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    if (y != NIL) {
        y->parent = x;
    }
}

void insertBalance(node *x)
{
    node *y = x;
    while (x != root) {
        if (x == root) {
            break;
        }
        y = x;
        x = x->parent;
        if (y == x->left) {
            x->balance++;
        } else {
            x->balance--;
        }
        if (x->balance == 0) {
            break;
        } else if (x->balance == 2) {
            y = x->left;
            if (y->balance == 1) {
                y->balance = 0;
                x->balance = 0;
                rightRotate(x);
                x->parent->balance++;
            } else if (y->balance == -1) {
                node *z = y->right;
                int oldBalance = z->balance;
                if (oldBalance == 1) {
                    z->balance = 0;
                    x->balance = -1;
                    y->balance = 0;
                } else if (oldBalance == -1) {
                    z->balance = 0;
                    x->balance = 0;
                    y->balance = 1;
                } else if (oldBalance == 0) {
                    z->balance = 0;
                    x->balance = 0;
                    y->balance = 0;
                }
                leftRotate(y);
                rightRotate(x);
                x->parent->balance++;
            }
        } else if (x->balance == -2) {
            y = x->right;
            if (y->balance == -1) {
                y->balance = 0;
                x->balance = 0;
                leftRotate(x);
                x->parent->balance--;
            } else if (y->balance == 1) {
                node *z = y->left;
                int oldBalance = z->balance;
                if (oldBalance == 1) {
                    z->balance = 0;
                    x->balance = 0;
                    y->balance = -1;
                } else if (oldBalance == -1) {
                    z->balance = 0;
                    x->balance = 1;
                    y->balance = 0;
                } else if (oldBalance == 0) {
                    z->balance = 0;
                    x->balance = 0;
                    y->balance = 0;
                }
                rightRotate(y);
                leftRotate(x);
                x->parent->balance--;
            }
        }
    }
}

void insert(char *key, int64_t value)
{
    node *x = root, *y = NIL, *z;
    while (x != NIL) {
        if (strcmp(key, x->key) == 0) {
            return;
        }
        y = x;
        if (strcmp(x->key, key) > 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    x = (node *) malloc(sizeof(node));
    x->key = (char *) malloc((strlen(key) + 1));
    memset(x->key, '\0', (strlen(key) + 1));
    memcpy(x->key, key, (strlen(key) + 1));
    x->value = value;
    x->balance = 0;
    x->parent = y;
    x->left = NIL;
    x->right = NIL;
    if (y != NIL) {
        if (strcmp(key, y->key) < 0) {
            y->left = x;
        } else {
            y->right = x;
        }
    } else {
        root = x;
    }
    insertBalance(x);
}

void delet(node *z) {
    node *x, *y;
    if (!z || z == NIL) {
        return;
    }
    if (z->left == NIL || z->right == NIL) {
        y = z;
    } else {
        y = z->right;
        while (y->left != NIL) {
            y = y->left;
        }
    }
    if (y->left != NIL) {
        x = y->left;
    } else {
        x = y->right;
    }
    x->parent = y->parent;
    if (y->parent != NIL) {
        if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
    } else {
        root = x;
    }
    if (y != z) {
        z->key = y->key;
        z->value = y->value;
    }
    if (root == y) {
        root = z;
    }
    y->key = NULL;
    free(y);
}

void print(node *t, int depth)
{
    if (t == NIL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }
    printf("%s:%ld^%d\n", t->key, t->value, t->balance);
    print(t->left, depth + 1);
    print(t->right, depth + 1);
}

node *destroy(node *t)
{
    if (t == NIL) {
        return t;
    }
    if (t->left == NIL && t->right == NIL) {
        free(t->key);
        t->key = NULL;
        free(t);
        return NIL;
    }
    t->left = destroy(t->left);
    t->right = destroy(t->right);
    root = NIL;
    return destroy(t);
}

int main(int argc, char const *argv[])
{
    // printf("%d\n", strcmp("a", "b") > 0);
    char key[256];
    // char *fileName;
    char command;
    uint64_t value;
    while (!feof(stdin)) {
        command = getchar();
        if (command == '+') {
            getchar();
            scanf("%s", key);
            for (int i = 0; i < strlen(key); i++) {
                key[i] = tolower(key[i]);
            }
            scanf("%ld", &value);
            node *t = search(key);
            if (t != NULL) {
                printf("Exist\n");
            } else {
                insert(key, value);
                printf("OK\n");
            }
        } else if (command == '-') {
            getchar();
            scanf("%s", key);
            for (int i = 0; i < strlen(key); i++) {
                key[i] = tolower(key[i]);
            }
            node *t = search(key);
            if (t != NULL) {
                delet(t);
                printf("OK\n");
            } else {
                printf("NoSuchWord\n");
            }
        } else if (command == '!') {

        } else if (command == '>') {
            print(root, 0);
        } else if (isalpha(command)){
            key[0] = command;
            scanf("%s", key + 1);
            for (int i = 0; i < strlen(key); i++) {
                key[i] = tolower(key[i]);
            }
            node *t = search(key);
            if (t != NULL) {
                printf("OK: %ld\n", t->value);
            } else {
                printf("NoSuchWord\n");
            }
        }
    }
    destroy(root);
    return 0;
}
