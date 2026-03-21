#pragma once

#include <type_traits>
#include <utility>
#include <vector>

#include "graph.hpp"


template <typename T = int>
class HLD {
    Graph<T> &g_;
    int n_;
    int root_;
    std::vector<int> sz_;
    std::vector<int> par_;
    std::vector<int> in_;
    std::vector<int> out_;
    std::vector<int> head_;
    std::vector<int> rev_;
    std::vector<int> dep_;
    std::vector<int> edge_in_;

    void dfs_sz(int v, int p) {
        sz_[v] = 1;
        par_[v] = p;
        for(auto &e : g_[v]) {
            if(e.to == p)
                std::swap(e, g_[v].back());
            if(e.to == p)
                continue;
            dep_[e.to] = dep_[v] + 1;
            dfs_sz(e.to, v);
            sz_[v] += sz_[e.to];
            if(sz_[e.to] > sz_[g_[v][0].to])
                std::swap(e, g_[v][0]);
        }
    }

    void dfs_hld(int v, int p, int &t) {
        in_[v] = t++;
        rev_[in_[v]] = v;
        for(auto e : g_[v]) {
            if(e.to == p) {
                if constexpr(std::is_integral_v<T>)
                    edge_in_[static_cast<int>(e.cost)] = in_[v];
                continue;
            }
            head_[e.to] = (e.to == g_[v][0].to ? head_[v] : e.to);
            dfs_hld(e.to, v, t);
        }
        out_[v] = t;
    }

  public:
    explicit HLD(Graph<T> &g, int root = 0)
        : g_(g),
          n_(static_cast<int>(g.size())),
          root_(root),
          sz_(n_),
          par_(n_, -1),
          in_(n_),
          out_(n_),
          head_(n_, -1),
          rev_(n_),
          dep_(n_, 0),
          edge_in_(n_ > 0 ? n_ - 1 : 0, 0) {
        dfs_sz(root_, -1);
        int t = 0;
        head_[root_] = root_;
        dfs_hld(root_, -1, t);
    }

    int get_in(int a) const { return in_[a]; }
    int get_sz(int a) const { return sz_[a]; }
    int get_rev(int a) const { return rev_[a]; }
    int get_dep(int a) const { return dep_[a]; }
    int get_edge_in(int a) const { return edge_in_[a]; }

    int lca(int a, int b) const {
        for(;; b = par_[head_[b]]) {
            if(in_[a] > in_[b])
                std::swap(a, b);
            if(head_[a] == head_[b])
                return a;
        }
    }

    std::pair<int, int> subtree_query(int a) const {
        return {in_[a], out_[a]};
    }

    std::vector<std::pair<int, int>> path_query(int a, int b, bool edge = false) const {
        std::vector<std::pair<int, int>> ret;
        for(;; b = par_[head_[b]]) {
            if(in_[a] > in_[b])
                std::swap(a, b);
            if(head_[a] == head_[b])
                break;
            ret.push_back({in_[head_[b]], in_[b] + 1});
        }
        ret.push_back({in_[a] + edge, in_[b] + 1});
        return ret;
    }
};

