#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    
    int m = 0, n = 0;
    std::cin >> m >> n;
    std::vector<std::vector<double>> v(m, std::vector<double>(n + 2, 0));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n + 1; j++) {
            std::cin >> v[i][j];
        }
        v[i][n + 1] = i + 1;
    }

    std::vector<int> minV;
    for (int i = 0; i < n; i++) {
        int minCost = 51;
        int selectRow = -1;
        for (int j = i; j < m; j++) {
            if (v[j][i] != 0 && v[j][n] < minCost) {
                minCost = v[j][n];
                selectRow = j;
            }
        }
        if (selectRow == -1) {
            std::cout << -1 << "\n";
            return 0;
        }
        std::swap(v[i], v[selectRow]);
        minV.push_back(v[i][n + 1]);
        for (int j = i + 1; j < m; j++) {
            double pivotDivisor = v[j][i] / v[i][i];
            for (int k = i; k < n; k++) {
                v[j][k] -= v[i][k] * pivotDivisor;
            }
        }
    }

    std::sort(minV.begin(), minV.end());
    for (int i = 0; i < minV.size(); i++) {
        std::cout << minV[i] << " ";
    }
    std::cout << "\n";
    return 0;
}
