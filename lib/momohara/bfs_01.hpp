#pragma once

#include <deque>
#include <limits>
#include <vector>

#include "graph.hpp"


template <typename T>
void bfs01(const Graph<T> &g, int s, std::vector<T> &d, T inf = std::numeric_limits<T>::max() / 4) {
    d.assign(g.size(), inf);
    d[s] = 0;
    std::deque<int> deq;
    deq.push_front(s);
    while(!deq.empty()) {
        int v = deq.front();
        deq.pop_front();
        for(const auto &e : g[v]) {
            if(d[v] + e.cost < d[e.to]) {
                d[e.to] = d[v] + e.cost;
                if(e.cost == 0) {
                    deq.push_front(e.to);
                } else {
                    deq.push_back(e.to);
                }
            }
        }
    }
}

template <typename T>
std::vector<T> bfs01(const Graph<T> &g, int s, T inf = std::numeric_limits<T>::max() / 4) {
    std::vector<T> d;
    bfs01(g, s, d, inf);
    return d;
}

