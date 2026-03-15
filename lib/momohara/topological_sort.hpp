#pragma once

#include <functional>
#include <queue>
#include <vector>

#include "graph.hpp"

namespace momohara {

template <typename T>
bool tsort(const Graph<T> &graph, std::vector<int> &order) {
    int n = static_cast<int>(graph.size());
    order.clear();
    std::vector<int> in(n);
    for(const auto &es : graph) {
        for(const auto &e : es)
            ++in[e.to];
    }
    std::priority_queue<int, std::vector<int>, std::greater<>> que;
    for(int i = 0; i < n; ++i) {
        if(in[i] == 0)
            que.push(i);
    }
    while(!que.empty()) {
        int v = que.top();
        que.pop();
        order.push_back(v);
        for(const auto &e : graph[v]) {
            if(--in[e.to] == 0)
                que.push(e.to);
        }
    }
    return static_cast<int>(order.size()) == n;
}

} // namespace momohara
