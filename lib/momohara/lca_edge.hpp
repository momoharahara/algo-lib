#pragma once

#include <algorithm>
#include <vector>

#include "graph.hpp"


template <typename F, class T, typename Cost = int>
class LCAEdge {
    int n_;
    int log2_n_;
    std::vector<std::vector<int>> parent_;
    std::vector<std::vector<T>> data_;
    std::vector<int> depth_;
    F op_;
    T id_;

  public:
    LCAEdge() : n_(0), log2_n_(0), id_() {}

    LCAEdge(const Graph<Cost> &g, int root, T id, F op)
        : n_(static_cast<int>(g.size())),
          log2_n_(1),
          depth_(n_, 0),
          op_(op),
          id_(id) {
        while((1 << log2_n_) <= n_)
            ++log2_n_;
        parent_.assign(log2_n_, std::vector<int>(n_, -1));
        data_.assign(log2_n_, std::vector<T>(n_, id_));
        dfs(g, root, -1, 0);
        for(int k = 0; k + 1 < log2_n_; ++k) {
            for(int v = 0; v < n_; ++v) {
                if(parent_[k][v] < 0) {
                    parent_[k + 1][v] = -1;
                    data_[k + 1][v] = data_[k][v];
                } else {
                    parent_[k + 1][v] = parent_[k][parent_[k][v]];
                    data_[k + 1][v] = op_(data_[k][parent_[k][v]], data_[k][v]);
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

    T get(int u, int v) const {
        if(depth_[u] > depth_[v])
            std::swap(u, v);
        T ret = id_;
        for(int k = 0; k < log2_n_; ++k) {
            if((depth_[v] - depth_[u]) >> k & 1) {
                ret = op_(ret, data_[k][v]);
                v = parent_[k][v];
            }
        }
        if(u == v)
            return ret;
        for(int k = log2_n_ - 1; k >= 0; --k) {
            if(parent_[k][u] != parent_[k][v]) {
                ret = op_(ret, data_[k][u]);
                u = parent_[k][u];
                ret = op_(ret, data_[k][v]);
                v = parent_[k][v];
            }
        }
        ret = op_(ret, data_[0][u]);
        ret = op_(ret, data_[0][v]);
        return ret;
    }

  private:
    void dfs(const Graph<Cost> &g, int v, int p, int d) {
        parent_[0][v] = p;
        depth_[v] = d;
        for(const auto &e : g[v]) {
            if(e.to == p) {
                data_[0][v] = static_cast<T>(e.cost);
            } else {
                dfs(g, e.to, v, d + 1);
            }
        }
    }
};

