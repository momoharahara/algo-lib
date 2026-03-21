#pragma once

#include <vector>

#include "low_link.hpp"


template <typename T = int>
struct TwoEdgeConnectedComponents : LowLink<T> {
    using LowLink<T>::LowLink;
    using LowLink<T>::bridge;
    using LowLink<T>::g;
    using LowLink<T>::low;
    using LowLink<T>::ord;

    std::vector<int> comp;
    Graph<T> tree;
    std::vector<std::vector<int>> group;

    explicit TwoEdgeConnectedComponents(const Graph<T> &graph) : LowLink<T>(graph) {}

    int operator[](int k) const {
        return comp[k];
    }

    void build() override {
        LowLink<T>::build();
        comp.assign(g.size(), -1);
        int k = 0;
        for(int i = 0; i < static_cast<int>(comp.size()); ++i) {
            if(comp[i] == -1)
                dfs(i, -1, k);
        }
        group.assign(k, {});
        for(int i = 0; i < static_cast<int>(g.size()); ++i)
            group[comp[i]].push_back(i);
        tree = Graph<T>(k);
        for(const auto &e : bridge)
            tree.add_edge(comp[e.from], comp[e.to], e.cost);
    }

  private:
    void dfs(int idx, int par, int &k) {
        if(par >= 0 && ord[par] >= low[idx]) {
            comp[idx] = comp[par];
        } else {
            comp[idx] = k++;
        }
        for(const auto &to : g[idx]) {
            if(comp[to] == -1)
                dfs(to, idx, k);
        }
    }
};

