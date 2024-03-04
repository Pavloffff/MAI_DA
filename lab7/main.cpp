#include <iostream>
#include <vector>
#include <algorithm>

class Node
{
public:
    uint64_t c;
    int i;
    Node *p;
    Node(uint64_t c, int i, Node *p);
    ~Node();
};

Node::Node(uint64_t c, int i, Node *p)
{
    this->c = c;
    this->i = i;
    this->p = p;
}

Node::~Node()
{
    if (p != nullptr) {
        delete p;
        p = nullptr;
    }
}

int main(int argc, char const *argv[])
{
    // std::ios::sync_with_stdio(0);
    // std::cin.tie(0);

    int n = 0, m = 0;
    std::cin >> n >> m;
    std::vector<int64_t> w(n);
    std::vector<int64_t> c(n);
    for (int i = 0; i < n; i++) {
        std::cin >> w[i] >> c[i];
    }

    std::vector<std::vector<std::vector<Node *>>> dp(
        m + 1,
        std::vector<std::vector<Node *>>(
            2,
            std::vector<Node *>(
                n + 1, 
                nullptr
            )
        )
    );
    Node *nil = new Node(0, -1, nullptr);

    for (int i = 0; i <= m; i++) {
        for (int j = 0; j < 2; j++) {
            dp[i][j][0] = nil;
        }
    }
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j <= m; j++) {
            for (int k = 1; k <= n; k++) {
                if (n - i < k) {
                    dp[j][i % 2][k] = nullptr;
                    continue;
                }

                if (w[i] > j) {
                    dp[j][i % 2][k] = dp[j][(i + 1) % 2][k];
                    continue;
                }

                Node *n0 = dp[j][(i + 1) % 2][k];
                Node *n1 = nullptr;

                if (w[i] <= j) {
                    Node *next = dp[j - w[i]][(i + 1) % 2][k - 1];
                    if (next != nullptr) {
                        n1 = new Node(next->c + c[i], i + 1, next);
                    }
                }

                if (n0 == nullptr) {
                    dp[j][i % 2][k] = n1;
                } else if (n1 == nullptr || n0->c > n1->c) {
                    dp[j][i % 2][k] = n0;
                } else {
                    dp[j][i % 2][k] = n1;
                }
            }
        }
    }

    uint64_t max = 0;
    Node *maxTmp = nullptr;

    for (int i = 0; i <= n; i++) {
        Node *node = dp[m][0][i];
        if (node == nullptr) {
            continue;
        }

        int c = node->c * i;
        if (c >= max) {
            maxTmp = node;
            max = c;
        }
    }

    std::vector<int> ansV;
    while (maxTmp != nullptr && maxTmp->i != -1) {
        ansV.push_back(maxTmp->i);
        maxTmp = maxTmp->p;
    }

    std::cout << max << "\n";
    if (max != 0) {
        for (int i = 0; i < ansV.size(); i++) {
            std::cout << ansV[i] << " ";
        }
        std::cout << "\n";
    }
    return 0;
}
