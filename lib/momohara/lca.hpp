#pragma once

#include <algorithm>
#include <vector>

#include "graph.hpp"


template <typename T = int>
class LCA {
    int n_;
    int log2_n_;
    std::vector<std::vector<int>> parent_;
    std::vector<int> depth_;

  public:
    LCA() : n_(0), log2_n_(0) {}

    LCA(const Graph<T> &g, int root)
        : n_(static_cast<int>(g.size())),
          log2_n_(1),
          depth_(n_, 0) {
        while((1 << log2_n_) <= n_)
            ++log2_n_;
        parent_.assign(log2_n_, std::vector<int>(n_, -1));
        dfs(g, root, -1, 0);
        for(int k = 0; k + 1 < log2_n_; ++k) {
            for(int v = 0; v < n_; ++v) {
                if(parent_[k][v] < 0) {
                    parent_[k + 1][v] = -1;
                } else {
                    parent_[k + 1][v] = parent_[k][parent_[k][v]];
                }
            }
        }
    }

    int get_par(int u, int i) const {
        for(int k = log2_n_ - 1; k >= 0; --k) {
            if((i >> k) & 1)
                u = parent_[k][u];
        }
        return u;
    }

    int get(int u, int v) const {
        if(depth_[u] > depth_[v])
            std::swap(u, v);
        for(int k = 0; k < log2_n_; ++k) {
            if((depth_[v] - depth_[u]) >> k & 1)
                v = parent_[k][v];
        }
        if(u == v)
            return u;
        for(int k = log2_n_ - 1; k >= 0; --k) {
            if(parent_[k][u] != parent_[k][v]) {
                u = parent_[k][u];
                v = parent_[k][v];
            }
        }
        return parent_[0][u];
    }

    int depth(int v) const {
        return depth_[v];
    }

  private:
    void dfs(const Graph<T> &g, int v, int p, int d) {
        parent_[0][v] = p;
        depth_[v] = d;
        for(const auto &e : g[v]) {
            if(e.to != p)
                dfs(g, e.to, v, d + 1);
        }
    }
};

