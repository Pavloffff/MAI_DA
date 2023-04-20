#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <unistd.h>
#include <map>
#include <fcntl.h>

#define String std::string

class Timer 
{
public:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;
    bool m_running = false;
    void start(); 
    void stop();
    double time();   
};

void Timer::start()
{
    this->m_startTime = std::chrono::high_resolution_clock::now();
    m_running = true;
}

void Timer::stop()
{
    m_endTime = std::chrono::high_resolution_clock::now();
    m_running = false;
}

double Timer::time()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    if (m_running) {
        endTime = std::chrono::high_resolution_clock::now();
    } else {
        endTime = m_endTime;
    }
    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_startTime).count();
}

class Tree
{
public:
    String key;
    unsigned long long value;
    Tree *left;
    Tree *right;
    unsigned int height;
    Tree();
    Tree(const String &key, unsigned long long value);
    ~Tree();
};

Tree::Tree()
{
}

Tree::Tree(const String &key, unsigned long long value)
{
    this->key = key;
    this->value = value;
    this->left = NULL;
    this->right = NULL;
    this->height = 1;
}

Tree::~Tree()
{
}

int getHeight(Tree *x)
{
    if (!x) {
        return 0;
    }
    return x->height;
}

Tree *leftRotate(Tree *x)
{
    Tree *y = x->right;
    x->right = y->left;
    y->left = x;
    if (getHeight(x->left) > getHeight(x->right)) {
        x->height = getHeight(x->left) + 1;
    } else {
        x->height = getHeight(x->right) + 1;
    }
    if (getHeight(y->left) > getHeight(y->right)) {
        y->height = getHeight(y->left) + 1;
    } else {
        y->height = getHeight(y->right) + 1;
    }
    return y;
}

Tree *rightRotate(Tree *x)
{
    Tree *y = x->left;
    x->left = y->right;
    y->right = x;
    if (getHeight(x->left) > getHeight(x->right)) {
        x->height = getHeight(x->left) + 1;
    } else {
        x->height = getHeight(x->right) + 1;
    }
    if (getHeight(y->left) > getHeight(y->right)) {
        y->height = getHeight(y->left) + 1;
    } else {
        y->height = getHeight(y->right) + 1;
    }
    return y;
}

bool find(Tree *x, String &key)
{
    if (x == NULL) {
        return false;
    } else if (key < x->key) {
        return find(x->left, key);
    } else if (key > x->key) {
        return find(x->right, key);
    } else {
        return true;
    }
}

int balance(Tree *x)
{
    if (x == NULL) {
        return 0;
    }
    return getHeight(x->left) - getHeight(x->right);
}

Tree *insert(Tree *x, String &key, unsigned long long value)
{
    if (x == NULL) {
        return new Tree(key, value);
    } else if (key < x->key) {
        x->left = insert(x->left, key, value);
    } else if (key > x->key) {
        x->right = insert(x->right, key, value);
    } else {
        return x;
    }
    if (getHeight(x->left) > getHeight(x->right)) {
        x->height = getHeight(x->left) + 1;
    } else {
        x->height = getHeight(x->right) + 1;
    }
    if (balance(x) > 1) {
        if (balance(x->left) < 0) {
            x->left = leftRotate(x->left);
        }
        return rightRotate(x);
    } else if (balance(x) < -1) {
        if (balance(x->right) > 0) {
            x->right = rightRotate(x->right);
        }
        return leftRotate(x);
    }
    return x;
}

Tree *successor(Tree *x)
{
    Tree *y = x->right;
    while (y->left != NULL) {
        y = y->left;
    }
    return y;
}

Tree *eraceMinimalNode(Tree *x)
{
    if (!x->left) {
        Tree *y = x->right;
        delete x;
        return y;
    }
    x->left = eraceMinimalNode(x->left);
    if (getHeight(x->left) > getHeight(x->right)) {
        x->height = getHeight(x->left) + 1;
    } else {
        x->height = getHeight(x->right) + 1;
    }
    if (balance(x) > 1) {
        if (balance(x->left) < 0) {
            x->left = leftRotate(x->left);
        }
        return rightRotate(x);
    } else if (balance(x) < -1) {
        if (balance(x->right) > 0) {
            x->right = rightRotate(x->right);
        }
        return leftRotate(x);
    }
    return x;
}

Tree *erace(Tree *x, String &key)
{
    if (x == NULL) {
        return x;
    } else if (key < x->key) {
        x->left = erace(x->left, key);
    } else if (key > x->key) {
        x->right = erace(x->right, key);
    } else {
        if (x->left == NULL && x->right == NULL) {
            delete x;
            return NULL;
        }
        if (!x->right) {
            Tree *y = x->left;
            delete x;
            return y;
        }
        x->key = successor(x)->key;
        x->value = successor(x)->value;
        x->right = eraceMinimalNode(x->right);
    }
    if (getHeight(x->left) > getHeight(x->right)) {
        x->height = getHeight(x->left) + 1;
    } else {
        x->height = getHeight(x->right) + 1;
    }
    if (balance(x) > 1) {
        if (balance(x->left) < 0) {
            x->left = leftRotate(x->left);
        }
        return rightRotate(x);
    } else if (balance(x) < -1) {
        if (balance(x->right) > 0) {
            x->right = rightRotate(x->right);
        }
        return leftRotate(x);
    }
    return x;
}

void print(Tree *x, int depth)
{
    if (x) {
        for (int i = 0; i < depth; i++) {
            std::cout << "\t";
        }
        std::cout << x->key << ":" << x->value << "^" << balance(x) << "\n";
        print(x->right, depth + 1);
        print(x->left, depth + 1);
    }
}

Tree *destroy(Tree *x)
{
    if (x == NULL) {
        return x;
    }
    if (x->left == NULL && x->right == NULL) {  
        delete x;
        return NULL;
    }
    x->left = destroy(x->left);
    x->right = destroy(x->right);
    return destroy(x);
}

int main(int argc, char const *argv[])
{
    FILE *file;
    if (argc > 1) {
        file = fopen(argv[1], "rw");
    }
    int n = 0;
    fscanf(file, "%d", &n);
    Timer timer;
    Tree *AVL = NULL;
    std::map<String, unsigned long long> RBT;

    timer.start();
    for (int i = 0; i < n; i++) {
        unsigned long long number;
        char s[257];
        fscanf(file, "%s", s);
        fscanf(file, "%llu", &number);
        String str(s);
        AVL = insert(AVL, str, number);
    }
    timer.stop();
    std::cout << "time of insertion of AVL-tree with " << n << " elements: " << timer.time() << "\n";
    
    fseek(file, 0, SEEK_SET);
    fscanf(file, "%d", &n);
    
    timer.start();
    for (int i = 0; i < n; i++) {
        unsigned long long number;
        char s[257];
        fscanf(file, "%s", s);
        fscanf(file, "%llu", &number);
        String str(s);
        RBT.insert({str, number});
    }
    timer.stop();
    std::cout << "time of insertion of std::map with " << n << " elements: " << timer.time() << "\n";
    
    fseek(file, 0, SEEK_SET);
    fscanf(file, "%d", &n);

    timer.start();
    for (int i = 0; i < n; i++) {
        unsigned long long number;
        char s[257];
        fscanf(file, "%s", s);
        fscanf(file, "%llu", &number);
        String str(s);
        find(AVL, str);
    }
    timer.stop();
    std::cout << "time of searching of AVL-tree with " << n << " elements: " << timer.time() << "\n";
    
    fseek(file, 0, SEEK_SET);
    fscanf(file, "%d", &n);
    
    timer.start();
    for (int i = 0; i < n; i++) {
        unsigned long long number;
        char s[257];
        fscanf(file, "%s", s);
        fscanf(file, "%llu", &number);
        String str(s);
        RBT.find(str);
    }
    timer.stop();
    std::cout << "time of searching of std::map with " << n << " elements: " << timer.time() << "\n";

    fseek(file, 0, SEEK_SET);
    fscanf(file, "%d", &n);

    timer.start();
    for (int i = 0; i < n; i++) {
        unsigned long long number;
        char s[257];
        fscanf(file, "%s", s);
        fscanf(file, "%llu", &number);
        String str(s);
        AVL = erace(AVL, str);
    }
    timer.stop();
    std::cout << "time of deletion of AVL-tree with " << n << " elements: " << timer.time() << "\n";
    
    fseek(file, 0, SEEK_SET);
    fscanf(file, "%d", &n);
    
    timer.start();
    for (int i = 0; i < n; i++) {
        unsigned long long number;
        char s[257];
        fscanf(file, "%s", s);
        fscanf(file, "%llu", &number);
        String str(s);
        RBT.erase(str);
    }
    timer.stop();
    std::cout << "time of deletion of std::map with " << n << " elements: " << timer.time() << "\n";

    destroy(AVL);
    if (argc > 1) {
        fclose(file);
    }
    return 0;
}