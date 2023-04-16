#include <iostream>

#define String int

int max(int x, int y)
{
    return (x > y) ? x : y;
}

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

class Tree
{
public:
    Node *root = NULL;
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
    printf("%d:%ld^%d\n", x->key, x->value, x->balance());
    this->print(x->left, depth + 1);
    this->print(x->right, depth + 1);
}

Node *Tree::destroy(Node *x)
{
    if (x == NULL) {
        return x;
    }
    if (x->left == NULL && x->right == NULL) {
        // destruct string
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
    t.print();
    t.insert(6, 2);
    t.print();
    t.insert(5, 3);
    t.print();
    t.insert(4, 4);
    t.print();
    t.insert(3, 5);
    t.print();
    t.insert(2, 6);
    t.print();
    t.insert(1, 7);
    t.print();
    t.insert(0, 7);
    t.print();
    t.erace(6);
    t.print();
    t.erace(4);
    t.print();
    t.erace(5);
    t.print();
    return 0;
}
