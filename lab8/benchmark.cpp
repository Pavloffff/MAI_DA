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
    return std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - m_startTime).count();
}

int main(int argc, char const *argv[])
{
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
    
    Timer timer;

    timer.start();
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
    timer.stop();
    std::cout << timer.time() << "\n";
    return 0;
}
