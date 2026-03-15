#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "graph.hpp"

namespace momohara {

template <typename T = int>
struct LowLink : Graph<T> {
    using Graph<T>::Graph;
    using Graph<T>::g;

    std::vector<int> ord;
    std::vector<int> low;
    std::vector<int> articulation;
    std::vector<Edge<T>> bridge;

    explicit LowLink(const Graph<T> &graph) : Graph<T>(graph) {}

    virtual void build() {
        used_.assign(g.size(), 0);
        ord.assign(g.size(), 0);
        low.assign(g.size(), 0);
        articulation.clear();
        bridge.clear();
        int k = 0;
        for(int i = 0; i < static_cast<int>(g.size()); ++i) {
            if(!used_[i])
                k = dfs(i, k, -1);
        }
    }

  private:
    std::vector<int> used_;

    int dfs(int idx, int k, int par) {
        used_[idx] = true;
        ord[idx] = k++;
        low[idx] = ord[idx];
        bool is_articulation = false;
        bool beet = false;
        int child_count = 0;
        for(const auto &to : g[idx]) {
            if(to == par && !std::exchange(beet, true))
                continue;
            if(!used_[to]) {
                ++child_count;
                k = dfs(to, k, idx);
                low[idx] = std::min(low[idx], low[to]);
                is_articulation |= par >= 0 && low[to] >= ord[idx];
                if(ord[idx] < low[to])
                    bridge.emplace_back(to);
            } else {
                low[idx] = std::min(low[idx], ord[to]);
            }
        }
        is_articulation |= par == -1 && child_count > 1;
        if(is_articulation)
            articulation.push_back(idx);
        return k;
    }
};

} // namespace momohara
