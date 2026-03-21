#pragma once

#include <functional>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

#include "graph.hpp"


template <typename T>
void Dijkstra(const Graph<T> &g, int s, std::vector<T> &d, T inf = std::numeric_limits<T>::max() / 4) {
    d.assign(g.size(), inf);
    d[s] = 0;
    std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>, std::greater<>> q;
    q.push({0, s});
    while(!q.empty()) {
        auto [dist, v] = q.top();
        q.pop();
        if(d[v] < dist)
            continue;
        for(const auto &e : g[v]) {
            if(d[e.to] > dist + e.cost) {
                d[e.to] = dist + e.cost;
                q.push({d[e.to], e.to});
            }
        }
    }
}

template <typename T>
std::vector<T> Dijkstra(const Graph<T> &g, int s, T inf = std::numeric_limits<T>::max() / 4) {
    std::vector<T> d;
    Dijkstra(g, s, d, inf);
    return d;
}

