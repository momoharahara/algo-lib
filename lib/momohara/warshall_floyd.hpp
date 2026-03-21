#pragma once

#include <algorithm>
#include <limits>
#include <vector>

#include "graph.hpp"


template <typename T>
void WarshallFloyd(const Graph<T> &g, std::vector<std::vector<T>> &dist, T inf = std::numeric_limits<T>::max() / 4) {
    int n = static_cast<int>(g.size());
    dist.assign(n, std::vector<T>(n, inf));
    for(int i = 0; i < n; ++i)
        dist[i][i] = 0;
    for(int i = 0; i < n; ++i) {
        for(const auto &e : g[i]) {
            dist[i][e.to] = std::min(dist[i][e.to], e.cost);
        }
    }
    for(int k = 0; k < n; ++k) {
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                if(dist[i][k] == inf || dist[k][j] == inf)
                    continue;
                dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
}

