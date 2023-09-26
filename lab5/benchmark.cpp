#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <unistd.h>
#include <fcntl.h>

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

namespace Ukkonen
{
    class SuffixTreeNode 
    {
    public:
        int start;
        int *end;
        std::map<char, SuffixTreeNode *> edges;
        int suffixNumber;
        SuffixTreeNode *suffixLink;
        int Length();
        SuffixTreeNode(int start, int *end, int suffixNumber, SuffixTreeNode *suffixLink);
        ~SuffixTreeNode();
    };

    int SuffixTreeNode::Length()
    {
        return *(end) - start + 1;
    }

    SuffixTreeNode::SuffixTreeNode(int start, int *end, int suffixNumber, SuffixTreeNode *suffixLink)
    {
        this->start = start;
        this->end = end; 
        this->suffixNumber = suffixNumber;
        this->suffixLink = suffixLink;
    }

    SuffixTreeNode::~SuffixTreeNode()
    {
        for (auto edge : edges) {
            delete edge.second;
        }
    }

    class SuffixTree
    {
    public:
        std::string text;
        SuffixTreeNode *root;
        SuffixTreeNode *lastCreatedNode;
        SuffixTreeNode *currentNode;
        int currentLength;
        int currentEdge;
        int cnt;
        int globalEnd;

        void Insert(int iter);
        void DestroyEndIndexes(SuffixTreeNode *node);
        SuffixTree(std::string &text);
        ~SuffixTree();
    };

    void SuffixTree::Insert(int iter)
    {
        lastCreatedNode = nullptr;
        globalEnd++;
        cnt++;

        while (cnt > 0) {
            if (currentLength == 0) {
                currentEdge = iter;
            }
            if (currentNode->edges.find(text[currentEdge]) == currentNode->edges.end()) {
                currentNode->edges[text[iter]] = new SuffixTreeNode(iter, &globalEnd, iter - cnt + 1, root);
                if (lastCreatedNode != nullptr) {
                    lastCreatedNode->suffixLink = currentNode;
                    lastCreatedNode = nullptr;
                } else {
                    lastCreatedNode = currentNode;
                }
            } else {
                SuffixTreeNode *childNode = currentNode->edges[text[currentEdge]];
                
                if (currentLength >= childNode->Length()) {
                    currentEdge += childNode->Length();
                    currentLength -= childNode->Length();
                    currentNode = childNode;
                    continue;
                }

                if (text[childNode->start + currentLength] == text[iter]) {
                    if (lastCreatedNode != nullptr 
                    // && currentNode != root
                    ) {
                        lastCreatedNode->suffixLink = currentNode;
                    }
                    currentLength++;
                    break;
                }

                int *splitPoint = NULL;
                splitPoint = new int;
                *splitPoint = (childNode->start + currentLength - 1);
                SuffixTreeNode *splitNode = new SuffixTreeNode(childNode->start, splitPoint, -1, root);
                currentNode->edges[text[currentEdge]] = splitNode;
                splitNode->edges[text[iter]] = new SuffixTreeNode(iter, &globalEnd, iter - cnt + 1, root);
                childNode->start += currentLength;
                splitNode->edges[text[childNode->start]] = childNode;

                if (lastCreatedNode != nullptr) {
                    lastCreatedNode->suffixLink = splitNode;
                }
                lastCreatedNode = splitNode;
            }
            
            cnt--;

            if (currentNode == root && currentLength > 0) {
                currentLength--;
                currentEdge = iter - cnt + 1;
            } else if (currentNode != root) {
                currentNode = currentNode->suffixLink;
            }
        }    
    }

    void SuffixTree::DestroyEndIndexes(SuffixTreeNode *node)
    {
        if (node->end == nullptr || node->end == &globalEnd) {
            return;
        }
        for (auto edge : node->edges) {
            DestroyEndIndexes(edge.second);
        }
        delete node->end;
        node->end = nullptr;
    }

    SuffixTree::SuffixTree(std::string &text)
    {
        currentLength = 0;
        currentEdge = -1;
        cnt = 0;
        globalEnd = -1;
        this->text = text + "{";
        lastCreatedNode = nullptr;
        root = new SuffixTreeNode(-1, new int(-1), -1, root);
        currentNode = root;
        for (int i = 0; i < text.size(); i++) {
            Insert(i);
        }
        Insert(text.length());
    }

    SuffixTree::~SuffixTree()
    {
        DestroyEndIndexes(root);
        delete root;
    }
}

namespace Naive
{
    class SuffixTreeNode 
    {
    public:
        std::string label;
        std::vector<SuffixTreeNode *> edges;
        uint32_t suffixNumber;
        SuffixTreeNode(std::string label, uint32_t suffixNumber);
        ~SuffixTreeNode();
    };

    SuffixTreeNode::SuffixTreeNode(std::string label, uint32_t suffixNumber)
    {
        this->label = label;
        this->suffixNumber = suffixNumber;
    }

    SuffixTreeNode::~SuffixTreeNode()
    {
        for (int i = 0; i < edges.size(); i++) {
            if (edges[i] != nullptr) {
                delete edges[i];
            }
        }
        edges.clear();
    }

    class SuffixTree
    {
    public:
        SuffixTreeNode *root;
        uint32_t lastNumber;

        SuffixTreeNode *Insert(std::string key);
        SuffixTreeNode *Insert(std::string key, SuffixTreeNode *node, int iter);
        SuffixTree();
        ~SuffixTree();
    };

    SuffixTreeNode *SuffixTree::Insert(std::string key)
    {
        for (int i = 0; i < root->edges.size(); i++) {
            if (root->edges[i]->label[0] == key[0]) {
                return Insert(key, root->edges[i], 0);
            }
        }
        SuffixTreeNode *node = new SuffixTreeNode(key, lastNumber);
        root->edges.push_back(node);
        lastNumber++;
        return node;
    }

    SuffixTreeNode *SuffixTree::Insert(std::string key, SuffixTreeNode *node, int iter)
    {
        for (int i = 0; i < node->label.size(); i++) {
            if (iter >= key.length()) { //NOTE: 3 кейс 
                return node;
            }
            if (key[iter] != node->label[i]) { //NOTE: 2 кейс
                SuffixTreeNode *node1 = new SuffixTreeNode(node->label.substr(i), node->suffixNumber);
                for (int j = 0; j < node->edges.size(); j++) {
                    node1->edges.push_back(node->edges[j]);
                }
                node->suffixNumber = 0;
                node->label = node->label.substr(0, i);
                SuffixTreeNode *node2 = new SuffixTreeNode(key.substr(iter), lastNumber);
                lastNumber++;
                node->edges.clear();
                node->edges.push_back(node1);
                node->edges.push_back(node2);
                return node2;
            }
            iter++;
        }

        if (iter >= key.length()) { //NOTE: 3 кейс
            return node;
        }

        for (int i = 0; i < node->edges.size(); i++) {
            if (node->edges[i]->label[0] == key[iter]) {
                return Insert(key, node->edges[i], iter);
            }
        }

        if (node->edges.size() == 0) {
            node->label += key.substr(iter); //NOTE: 2 кейс
            return node;
        } else {
            SuffixTreeNode *node1 = new SuffixTreeNode(key.substr(iter), lastNumber);
            lastNumber++;
            node->edges.push_back(node1);
            return node1;
        }
    }

    SuffixTree::SuffixTree()
    {
        root = new SuffixTreeNode("@", 0);
        lastNumber = 1;
    }

    SuffixTree::~SuffixTree()
    {
        delete root;
    }
}

int main(int argc, char const *argv[])
{
    std::string text;
    std::cin >> text;
    Timer timer;

    timer.start();
    Ukkonen::SuffixTree ukkonenTree(text);
    timer.stop();

    std::cout << "Time of construction Suffix Tree (Ukkonen algorithm) is " << timer.time() << "\n";

    // timer.start();
    // Naive::SuffixTree naiveTree;
    // text += "$";
    // for (int i = 0; i < text.length(); i++) {
    //     std::string prefix = text.substr(0, i + 1);
    //     for (int j = 0; j < prefix.size(); j++) {
    //         Naive::SuffixTreeNode *n = naiveTree.Insert(prefix.substr(j));
    //     }
    // }
    // timer.stop();

    // std::cout << "Time of construction Suffix Tree (Naive algorithm) is " << timer.time() << "\n";

    return 0;
}
