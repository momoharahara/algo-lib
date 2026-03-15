#pragma once

#include <limits>
#include <vector>

#include "graph.hpp"

namespace momohara {

template <typename T>
void BellmanFord(
    const Graph<T> &g,
    int s,
    std::vector<T> &dist,
    std::vector<bool> &negative,
    T inf = std::numeric_limits<T>::max() / 4) {
    int n = static_cast<int>(g.size());
    dist.assign(n, inf);
    negative.assign(n, false);
    dist[s] = 0;
    for(int k = 0; k < n - 1; ++k) {
        for(int i = 0; i < n; ++i) {
            for(const auto &e : g[i]) {
                if(dist[i] != inf && dist[e.to] > dist[i] + e.cost) {
                    dist[e.to] = dist[i] + e.cost;
                }
            }
        }
    }
    for(int k = 0; k < n - 1; ++k) {
        for(int i = 0; i < n; ++i) {
            for(const auto &e : g[i]) {
                if(dist[i] != inf && dist[e.to] > dist[i] + e.cost) {
                    dist[e.to] = dist[i] + e.cost;
                    negative[e.to] = true;
                }
                if(negative[i])
                    negative[e.to] = true;
            }
        }
    }
}

} // namespace momohara
