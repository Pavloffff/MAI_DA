#include <iostream>
#include <string.h>

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
    uint64_t value;
    Node *left;
    Node *right;
    int height;
    int getHeight();
    int balance();
    Node *leftRotate();
    Node *rightRotate();
    Node *sucsessor();
    Node();
    Node(String key, uint64_t value);
    ~Node();
};

Node::Node()
{
}

Node::Node(String key, uint64_t value)
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
    Node *find(String key);
    Node *find(Node *x, String key);
    void insert(String key, uint64_t value);
    Node *insert(Node *x, String key, uint64_t value);
    void erace(String key);
    Node *erace(Node *x, String key);
    void print();
    void print(Node *x, int depth);
    Node *destroy(Node *x);
    Tree();
    ~Tree();
};

Node *Tree::find(String key)
{
    return this->find(this->root, key);
}

Node *Tree::find(Node *x, String key)
{
    if (x == NULL) {
        return NULL;
    } else if (x->key > key) {
        return find(x->left, key);
    } else if (x->key < key) {
        return find(x->right, key);
    } else {
        return x;
    }
}

void Tree::insert(String key, uint64_t value)
{
    this->root = this->insert(root, key, value);
}

Node *Tree::insert(Node *x, String key, uint64_t value)
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

void Tree::erace(String key)
{
    this->root = this->erace(root, key); 
}

Node *Tree::erace(Node *x, String key)
{
    if (x == NULL) {
        printf("NoSuchWord\n");
        return x;
    }
    if (key < x->key) {
        x->left = this->erace(x->left, key);
    } else if (key > x->key) {
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
            x->right = this->erace(x->right, tmp->key); 
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
    printf("%s:%ld^%d\n", x->key.data, x->value, x->balance());
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

Tree::Tree()
{
    root = NULL;
}

Tree::~Tree()
{
    this->destroy(root);
    root = NULL;
}

int main(int argc, char const *argv[])
{
    Tree t;
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
            String Key(key);
            Node *x = t.find(Key);
            if (x != NULL) {
                printf("Exist\n");   
            } else {
                t.insert(Key, value);
                printf("OK\n");
            }
        } else if (command == '-') {
            getchar();
            scanf("%s", key);
            for (int i = 0; i < strlen(key); i++) {
                key[i] = tolower(key[i]);
            }
            String Key(key);
            Node *x = t.find(Key);
            if (x == NULL) {
                t.erace(key);
                printf("OK\n");
            } else {
                printf("NoSuchWord\n");
            }
        } else if (command == '!') {

        } else if (command == '>') {
            t.print();
        } else if (isalpha(command)) {
            key[0] = command;
            int iter = 1;
            while ((command = getchar()) != '\n') {
                key[iter] = command;
                iter++;
            }
            for (int i = 0; i < 256; i++) {
                if (key[i] == '\0') {
                    break;
                }
                key[i] = tolower(key[i]);
            }
            String Key(key);
            Node *x = t.find(Key);
            if (x != NULL) {
                printf("OK: %ld\n", x->value);
            } else {
                printf("NoSuchWord\n");
            }
        }
        memset(key, '\0', 256);
    }
    return 0;
}
