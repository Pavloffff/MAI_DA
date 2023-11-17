#include <iostream>
#include <vector>
#include <algorithm>

void dfs(std::vector<std::vector<int>> &graph, std::vector<int> &visited, std::vector<int> &component, int u)
{
    visited[u] = 1;
    component.push_back(u);
    for (auto i: graph[u]) {
        if (!visited[i]) {
            dfs(graph, visited, component, i);
        }
    }
}

int main(int argc, char const *argv[])
{
    int n = 0, m = 0;
    std::cin >> n >> m;
    std::vector<std::vector<int>> graph(n);
    for (int i = 0; i < m; i++) {
        int u = 0, v = 0;
        std::cin >> u >> v;
        --u;
        --v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    std::vector<int> visited(n);
    std::vector<std::vector<int>> components;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            components.push_back(std::vector<int>());
            dfs(graph, visited, components[components.size() - 1], i);
            std::sort(components[components.size() - 1].begin(), components[components.size() - 1].end());
        }
    }
    
    std::sort(components.begin(), components.end());
    for (int i = 0; i < components.size(); i++) {
        for (int j = 0; j < components[i].size(); j++) {
            std::cout << components[i][j] + 1 << " ";
        }
        std::cout << "\n";
    }
    return 0;
}
