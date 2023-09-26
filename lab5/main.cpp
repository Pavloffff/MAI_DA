#include <iostream>
#include <vector>
#include <string>
#include <map>

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
    std::string CycleStringLinearization();
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

std::string SuffixTree::CycleStringLinearization()
{
    std::string output;
    int length = (text.size() / 2);
    SuffixTreeNode *current = root;
    int size = 0;
    while (true) {
        for (int i = 0; i < 28; i++) {
            if (current->edges.find((char)('a' + i)) != current->edges.end()) {
                for (int j = current->edges[(char)('a' + i)]->start; j <= *(current->edges[(char)('a' + i)]->end); j++) {
                    if (size < length) {
                        output += text[j];
                    } else {
                        return output;
                    }
                    size++;
                }
                current = current->edges[(char)('a' + i)];
                break;
            }
        }
    }
    return output;
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

int main(int argc, char const *argv[])
{
    std::string text;
    std::cin >> text;
    text = text + text;
    SuffixTree tree = SuffixTree(text);
    std::cout << tree.CycleStringLinearization() << "\n";    
    return 0;
}
