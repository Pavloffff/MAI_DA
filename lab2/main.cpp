#include <iostream>
#include <string.h>

char NODE = '@';
char END = '$';

class String
{
public:
    char data[257] = {'\0'};
    int sz = 0;
    int size();
    String &operator=(const String &s);
    String &operator=(const char *s);
    bool operator<(const String &s) const;
    bool operator>(const String &) const;
    String();
    String(const char *s);
    String(const String &s);
    ~String();
};

String::String()
{
}

String::String(const char *s)
{
    while (s[sz] != '\0') {
        data[sz] = s[sz];
        sz++;
    }
    for (int i = 0; i < sz; ++i) {
		if (data[i] >= 'A' && data[i] <= 'Z') {
			data[i] = (data[i] - ('A'-'a'));
		}
	}
}

String::String(const String &s)
{
    *this = s;
}

int String::size()
{
    return sz;
}

String &String::operator=(const String &s)
{
    sz = s.sz;
    memcpy(data, s.data, sz + 1);
    return *this;
}

String &String::operator=(const char *s)
{
    sz = 0;
    while (s[sz] != '\0') {
        data[sz] = s[sz];
        sz++;
    }
    data[sz] = s[sz];
    for (int i = 0; i < sz; ++i) {
		if (data[i] >= 'A' && data[i] <= 'Z') {
			data[i] = (data[i] - ('A'-'a'));
		}
	}
    return *this;
}

bool String::operator<(const String &s) const
{
    return strcmp(data, s.data) < 0;
}

bool String::operator>(const String &s) const
{
    return strcmp(data, s.data) > 0;
}

String::~String()
{
}

std::istream& operator>>(std::istream &cin, String &s) {
    s.data[0] = '\0';
    cin >> s.data;
    s.sz = 0;
    while (s.data[s.sz] != '\0') {
        ++s.sz;
    }
    for (int i = 0; i < s.sz; ++i) {
		if (s.data[i] >= 'A' && s.data[i] <= 'Z') {
			s.data[i] = (s.data[i] - ('A'-'a'));
		}
	}
    return cin;
}

std::ostream& operator<<(std::ostream &cout, const String &s) {
    cout << s.data;
    return cout;
}

class Tree
{
public:
    String key;
    unsigned long long value;
    Tree *left;
    Tree *right;
    unsigned int height;
    int getHeight(Tree *x);
    int balance(Tree *x);
    Tree *leftRotate(Tree *x);
    Tree *rightRotate(Tree *x);
    Tree *successor(Tree *x);
    bool find(Tree *x, String &key);
    Tree *insert(Tree *x, String &key, unsigned long long value);
    Tree *erace(Tree *x, String &key);
    void save(Tree *x, FILE *f);
    void load(Tree *&x, FILE *f);
    void print(Tree *x, int depth);
    Tree *destroy(Tree *x);
    Tree();
    Tree(String &key, unsigned long long value);
    ~Tree();
};

Tree::Tree()
{
}

Tree::Tree(String &key, unsigned long long value)
{
    this->key = key;
    this->value = value;
    this->left = NULL;
    this->right = NULL;
    this->height = 1;
}

int Tree::getHeight(Tree *x)
{
    if (!x) {
        return 0;
    }
    return x->height;
}

Tree *Tree::leftRotate(Tree *x)
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

Tree *Tree::rightRotate(Tree *x)
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

Tree *Tree::successor(Tree *x)
{
    if (x == NULL) {
        return x;
    }
    Tree *y = x->right;
    if (y == NULL) {
        return x;
    } else {
        while (y->left != NULL) {
            y = y->left;
        }
        return y;
    }
}

bool Tree::find(Tree *x, String &key)
{
    if (x == NULL) {
        std::cout << "NoSuchWord\n";
        return false;
    } else if (key < x->key) {
        return find(x->left, key);
    } else if (key > x->key) {
        return find(x->right, key);
    } else {
        std::cout << "OK: " << x->value << "\n";
        return true;
    }
}

int Tree::balance(Tree *x)
{
    if (x == NULL) {
        return 0;
    }
    return getHeight(x->left) - getHeight(x->right);
}

Tree *Tree::insert(Tree *x, String &key, unsigned long long value)
{
    if (x == NULL) {
        std::cout << "OK\n";
        return new Tree(key, value);
    } else if (key < x->key) {
        x->left = insert(x->left, key, value);
    } else if (key > x->key) {
        x->right = insert(x->right, key, value);
    } else {
        std::cout << "Exist\n";
        return x;
    }
    if (getHeight(x->left) > getHeight(x->right)) {
        x->height = getHeight(x->left) + 1;
    } else {
        x->height = getHeight(x->right) + 1;
    }
    if (balance(x) > 1) {
        if (balance(x->right) <= 0) {
            return rightRotate(x);
        } else if (key > x->left->key) {
            x->left = leftRotate(x->left);
            return rightRotate(x);
        }
    }
    if (balance(x) < -1) {
        if (balance(x->right) <= 0) {
            return leftRotate(x);
        } else if (key < x->right->key) {
            x->right = rightRotate(x->right);
            return leftRotate(x);
        }
    }
    return x;
}

Tree *Tree::erace(Tree *x, String &key)
{
    if (x == NULL) {
        std::cout << "NoSuchWord\n";
        return x;
    } else if (key < x->key) {
        x->left = erace(x->left, key);
    } else if (key > x->key) {
        x->right = erace(x->right, key);
    } else {
        if (x->left == NULL || x->right == NULL) {
            Tree *y = NULL;
            if (x->left != NULL) {
                y = x->left;
            } else {
                y = x->right;
            }
            if (y == NULL) {
                y = x;
                x = NULL;
            } else {
                *x = *y;
            }
            std::cout << "OK\n";
            delete y;
        } else {
            Tree *y = successor(x);
            x->key = y->key;
            x->value = y->value;
            x->right = erace(x->right, y->key);
        }
    }
    if (x == NULL) {
        return x;
    }
    if (getHeight(x->left) > getHeight(x->right)) {
        x->height = getHeight(x->left) + 1;
    } else {
        x->height = getHeight(x->right) + 1;
    }
    if (balance(x) > 1) {
        if (balance(x->right) <= 0) {
            return rightRotate(x);
        } else if (key > x->left->key) {
            x->left = leftRotate(x->left);
            return rightRotate(x);
        }
    }
    if (balance(x) < -1) {
        if (balance(x->right) <= 0) {
            return leftRotate(x);
        } else if (key < x->right->key) {
            x->right = rightRotate(x->right);
            return leftRotate(x);
        }
    }
    return x;
}

void Tree::print(Tree *x, int depth) {
    if (x) {
        for (int i = 0; i < depth; i++) {
            std::cout << "\t";
        }
        std::cout << x->key << ":" << x->value << "^" << x->balance(x) << "\n";
        print(x->right, depth + 1);
        print(x->left, depth + 1);
    }
}

Tree *Tree::destroy(Tree *x)
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

void Tree::save(Tree *x, FILE *f)
{
    if (x == NULL) {
        fwrite(&END, sizeof(char), 1, f);
        return;
    }
    size_t sz = x->key.size();
    fwrite(&NODE, sizeof(char), 1, f);
    fwrite(&sz, sizeof(sz), 1, f);
    fwrite(x->key.data, sizeof(char), sz, f);
    fwrite(&x->value, sizeof(unsigned long long), 1, f);
    fwrite(&x->height, sizeof(int), 1, f);
    save(x->left, f);
    save(x->right, f);
}

void Tree::load(Tree *&x, FILE *f)
{
    char c;
    size_t data = fread(&c, sizeof(char), 1, f);
    if (c == NODE) {
        char k[257] = {'\0'};
        unsigned long long value;
        size_t sz;
        int height;
        data = fread(&sz, sizeof(size_t), 1, f);
        data = fread(k, sizeof(char), sz, f);
        data = fread(&value, sizeof(unsigned long long), 1, f);
        data = fread(&height, sizeof(int), 1, f);
        for (int i = 1; i < data; i *= 1000) {};
        String key(k);
        x = new Tree(key, value);
        x->height = height;
    } else {
        return;
    }
    load(x->left, f);
    load(x->right, f);
}

Tree::~Tree()
{
}

int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    String command;
    Tree *t = NULL;
    while (std::cin >> command) {
        if (strcmp(command.data, "+") == 0) {
            unsigned long long value;
            std::cin >> command >> value;
            t = t->insert(t, command, value);
        } else if (strcmp(command.data, "-") == 0) {
            std::cin >> command;
            t = t->erace(t, command);
        } else if (strcmp(command.data, "!") == 0) {
            std::cin >> command;
            if (strcmp(command.data, "save") == 0) {
                std::cin >> command;
                FILE *f = fopen(command.data, "wb");
                t->save(t, f);
                fclose(f);
                std::cout << "OK\n";
            } else {
                std::cin >> command;
                FILE *f = fopen(command.data, "rb");
                if (f == NULL) {
                    t->destroy(t);
                    t = NULL;
                } else {
                    if (t != NULL) {
                        t->destroy(t);
                        t = NULL;
                    }
                    t->load(t, f);
                    fclose(f);
                }
                std::cout << "OK\n";
            }
        } else {
            bool x = t->find(t, command);
        }
        // t->print(t, 0);
    }
    t->destroy(t);
    return 0;
}
