#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

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
    return std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - m_startTime).count();
}

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

    Timer timer;

    timer.start();
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
    
    timer.stop();
    
    std::cout << timer.time() << std::endl;

    return 0;
}
