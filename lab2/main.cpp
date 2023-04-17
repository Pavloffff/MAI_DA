#include <iostream>
#include <string.h>

char NODE = '@';
char END = '$';

// class String

class String
{
public:
    char *data = (char *) NULL;
    size_t size();
    String &operator=(const String &s);
    bool operator<(const String &s);
    bool operator>(const String &s);
    String();
    String(char *s);
    String(const String &s);
    ~String();
};

String::String()
{
}

String::String(char *s)
{
    this->data = new char[strlen(s) + 1];
    memset(this->data, '\0', strlen(s) + 1);
    memcpy(this->data, s, strlen(s) + 1);
}

String::String(const String &s)
{
    this->data = new char[strlen(s.data) + 1];
    memset(this->data, '\0', strlen(s.data) + 1);
    memcpy(this->data, s.data, strlen(s.data) + 1);
}

size_t String::size()
{
    return strlen(this->data);
}

String &String::operator=(const String &s)
{
    if (this->data != (char *) NULL) {
        delete[] this->data;
    }
    this->data = new char[strlen(s.data) + 1];
    memset(this->data, '\0', strlen(s.data) + 1);
    memcpy(this->data, s.data, strlen(s.data) + 1);
    return *this;
}

bool String::operator<(const String &s)
{
    return strcmp(this->data, s.data) < 0;
}

bool String::operator>(const String &s)
{
    return strcmp(this->data, s.data) > 0;
}

String::~String()
{
    delete[] this->data;
}

// function max

int max(int x, int y)
{
    return (x > y) ? x : y;
}

// class Node

class Node
{
public:
    String key;
    unsigned long long value;
    Node *left;
    Node *right;
    int height;
    int getHeight();
    int balance();
    Node *leftRotate();
    Node *rightRotate();
    Node *sucsessor();
    Node();
    Node(String key, unsigned long long value);
    ~Node();
};

Node::Node()
{
}

Node::Node(String key, unsigned long long value)
{
    this->key = key;
    this->value = value;
    this->height = 1;
    this->left = NULL;
    this->right = NULL;
}

int Node::getHeight()
{
    if (this == NULL) {
        return 0;
    }
    return this->height;
}

int Node::balance()
{
    if (this == NULL) {
        return 0;
    }
    return this->left->getHeight() - this->right->getHeight();
}

Node *Node::leftRotate()
{
    Node *x = this->right;
    Node *y = x->left;
    x->left = this;
    this->right = y;
    this->height = max(this->left->getHeight(), this->right->getHeight()) + 1;
    x->height = max(x->left->getHeight(), x->right->getHeight()) + 1;
    return x;
}

Node *Node::rightRotate()
{
    Node *x = this->left;
    Node *y = x->right;
    x->right = this;
    this->left = y;
    this->height = max(this->left->getHeight(), this->right->getHeight()) + 1;
    x->height = max(x->left->getHeight(), x->right->getHeight()) + 1;
    return x;
}

Node *Node::sucsessor()
{
    Node *tmp = this->right;
    while (tmp->left != NULL) {
        tmp = tmp->left;
    }
    return tmp;
}

Node::~Node()
{
}

// class Tree

class Tree
{
public:
    Node *root = NULL;
    Node *find(char *key);
    Node *find(Node *x, char *key);
    void insert(String key, unsigned long long value);
    Node *insert(Node *x, String key, unsigned long long value);
    void erace(char *key);
    Node *erace(Node *x, char *key);
    bool save(char *fileName);
    bool save(FILE *f, Node *x);
    bool load(char *fileName);
    bool load(FILE *f, Node *&x);
    void print();
    void print(Node *x, int depth);
    Node *destroy(Node *x);
    Tree();
    ~Tree();
};

Node *Tree::find(char *key)
{
    return this->find(this->root, key);
}

Node *Tree::find(Node *x, char *key)
{
    if (x == NULL) {
        return NULL;
    } else if (strcmp(x->key.data, key) > 0) {
        return find(x->left, key);
    } else if (strcmp(x->key.data, key) < 0) {
        return find(x->right, key);
    } else {
        return x;
    }
}

void Tree::insert(String key, unsigned long long value)
{
    this->root = this->insert(root, key, value);
}

Node *Tree::insert(Node *x, String key, unsigned long long value)
{    
    if (x == NULL) {
        return new Node(key, value);
    } else if (x->key > key) {
        x->left = insert(x->left, key, value);
    } else if (x->key < key) {
        x->right = insert(x->right, key, value);
    } else {
        return x;
    }
    x->height = 1 + max(x->left->getHeight(), x->right->getHeight());
    int balance = x->balance();
    if (balance > 1) {
        if (key < x->left->key) {
            return x->rightRotate();
        } else if (key > x->left->key) {
            x->left = x->left->leftRotate();
            return x->rightRotate();
        }
    }
    if (balance < -1) {
        if (key > x->right->key) {
            return x->leftRotate();
        } else if (key < x->right->key) {
            x->right = x->right->rightRotate();
            return x->leftRotate();
        }
    }
    return x;
}

void Tree::erace(char *key)
{
    this->root = this->erace(root, key); 
}

Node *Tree::erace(Node *x, char *key)
{
    if (x == NULL) {
        printf("NoSuchWord\n");
        return x;
    }
    if (strcmp(key, x->key.data) < 0) {
        x->left = this->erace(x->left, key);
    } else if (strcmp(key, x->key.data) > 0) {
        x->right = this->erace(x->right, key);
    } else {
        if (x->left == NULL || x->right == NULL) {
            Node *tmp = x->left != NULL ? x->left : x->right;
            if (tmp == NULL) {
                tmp = x;
                x = NULL;
            } else {
                *x = *tmp;
            }
            delete tmp;
        } else {
            Node *tmp = x->sucsessor();
            x->key = tmp->key;
            x->value = tmp->value;
            x->right = this->erace(x->right, tmp->key.data); 
        }
    }
    if (x == NULL) {
        return x;
    }
    x->height = 1 + max(x->left->getHeight(), x->right->getHeight());
    int balance = x->balance();
    if (balance > 1) {
        if (x->right->balance() <= 0) {
            return x->rightRotate();
        } else {
            x->right = x->right->leftRotate();
            return x->rightRotate();
        }
    }
    if (balance < -1) {
        if (x->right->balance() <= 0) {
            return x->leftRotate();
        } else {
            x->right = x->right->rightRotate();
            return x->leftRotate();
        }
    }
    return x;
}

void Tree::print()
{
    this->print(this->root, 0);
}

void Tree::print(Node *x, int depth)
{
    if (x == NULL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }
    printf("%s:%llu^%d\n", x->key.data, x->value, x->balance());
    this->print(x->left, depth + 1);
    this->print(x->right, depth + 1);
}

Node *Tree::destroy(Node *x)
{
    if (x == NULL) {
        return x;
    }
    if (x->left == NULL && x->right == NULL) {
        
        delete x;
        return NULL;
    }
    x->left = this->destroy(x->left);
    x->right = this->destroy(x->right);
    return destroy(x);
}

bool Tree::save(char *fileName)
{
    FILE* f = fopen(fileName, "wb");
    if (f == NULL) {
        return false;
    }
    if (this->root == NULL) {
        if (fwrite(&END, sizeof(char), 1, f) != 1) {
            return false;
        }
        return true;
    }
    bool check = save(f, this->root);
    fclose(f);
    return check;
}

bool Tree::save(FILE *f, Node *x)
{
    if (x == NULL) {
        if (fwrite(&END, sizeof(char), 1, f) != 1) {
            return false;
        }
        return true;
    }
    size_t sz = strlen(x->key.data);
    fwrite(&NODE, sizeof(char), 1, f);
    fwrite(&sz, sizeof(sz), 1, f);
    fwrite(x->key.data, sizeof(char), sz, f);
    fwrite(&x->value, sizeof(unsigned long long), 1, f);
    fwrite(&x->height, sizeof(int), 1, f);
    if (!save(f, x->left)) {
        return false;
    }
    if (!save(f, x->right)) {
        return false;
    }
    return true;
}

bool Tree::load(char *fileName)
{
    FILE* f = fopen(fileName, "rb");
    if (f == NULL) {
        fclose(f);
        return false;
    }
    if (this->root != NULL) {
        this->destroy(this->root);
        this->root = NULL;
    }
    bool check = load(f, root);
    fclose(f);
    return check;
}

bool Tree::load(FILE *f, Node *&x)
{
    char c;
    size_t data = fread(&c, sizeof(char), 1, f);
    if (c == NODE) {
        char *k = new char[257];
        memset(k, '\0', 256);
        unsigned long long value;
        size_t sz;
        int height;
        data = fread(&sz, sizeof(size_t), 1, f);
        data = fread(k, sizeof(char), sz, f);
        data = fread(&value, sizeof(unsigned long long), 1, f);
        data = fread(&height, sizeof(int), 1, f);
        for (int i = 1; i < data; i *= 1000) {};
        String key(k);
        x = new Node(key, value);
        x->height = height; 
    } else {
        return true;
    }
    if (!load(f, x->left)) {
        return false;
    }
    if (!load(f, x->right)) {
        return false;
    }
    return true;
}

Tree::Tree()
{
    root = NULL;
}

Tree::~Tree()
{
    this->destroy(root);
    root = NULL;
}

// int main

int main(int argc, char const *argv[])
{
    Tree *t = new Tree();
    char command;
    char *key = new char[257];
    while (std::cin >> command) {
        memset(key, '\0', 257);
        if (command == '+') {
            unsigned long long int value;
            std::cin >> key >> value;
            for (int i = 0; i < 257; i++) {
                key[i] = tolower(key[i]);
            }
            Node *x = t->find(key);
            if (x == NULL) {
                String Key(key);
                t->insert(Key, value);
                std::cout << "OK\n";
            } else {
                std::cout << "Exists\n";
            }
        } else if (command == '-') {
            std::cin >> key;
            for (int i = 0; i < 257; i++) {
                key[i] = tolower(key[i]);
            }
            Node *x = t->find(key);
            if (x == NULL) {
                std::cout << "NoSuchWord\n";
            } else {
                t->erace(key);
                std::cout << "OK\n";
            }
        } else if (command == '!') {
            std::cin.get(command);
            std::cin.get(command);
            if (command == 'S') {
                while (std::cin.get(command)) {
                    if (command == ' ') {
                        break;
                    }
                }
                std::cin >> key;
                t->save(key);
                std::cout << "OK\n";
            } else if (command == 'L') {
                while (std::cin.get(command)) {
                    if (command == ' ') {
                        break;
                    }
                }
                std::cin >> key;
                if (t->load(key)) {
                } else {
                    t->destroy(t->root);
                    t->root = NULL;
                }
                std::cout << "OK\n";
            }
        } else if (isalpha(command)) {
            key[0] = command;
            int iter = 1;
            while (std::cin.get(command)) {
                if (command == '\n') {
                    break;
                }
                key[iter] = command;
                iter++;
            }
            for (int i = 0; i < 257; i++) {
                key[i] = tolower(key[i]);
            }
            Node *x = t->find(key);
            if (x == NULL) {
                std::cout << "NoSuchWord\n";
            } else {
                std::cout << "OK: " << x->value << "\n";
            }
        } else if (command == '>') {
            t->print();
        }
    }
    delete[] key;
    delete t;
    return 0;
}
