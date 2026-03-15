#pragma once

#include <set>
#include <utility>
#include <vector>

#include "graph.hpp"

namespace momohara {

template <typename T = int>
class SCC {
    Graph<T> g_;
    Graph<T> rg_;
    std::vector<int> cmp_;
    std::vector<int> used_;
    std::vector<int> vs_;
    int size_;

  public:
    explicit SCC(const Graph<T> &g) : g_(g), rg_(g.size()), cmp_(g.size(), -1), used_(g.size(), false), size_(0) {
        for(int i = 0; i < static_cast<int>(g_.size()); ++i) {
            for(const auto &e : g_[i])
                rg_.add_directed_edge(e.to, i, e.cost);
        }

        for(int i = 0; i < static_cast<int>(g_.size()); ++i) {
            if(!used_[i])
                dfs(i);
        }
        int k = 0;
        for(int i = static_cast<int>(vs_.size()) - 1; i >= 0; --i) {
            int v = vs_[i];
            if(used_[v]) {
                rdfs(v, k);
                ++k;
            }
        }
        size_ = k;
    }

    int getSize() const {
        return size_;
    }

    int group(int v) const {
        return cmp_[v];
    }

    const std::vector<int> &groups() const {
        return cmp_;
    }

    Graph<T> rebuild() const {
        Graph<T> ret(size_);
        std::set<std::pair<int, int>> st;
        for(int i = 0; i < static_cast<int>(g_.size()); ++i) {
            int u = cmp_[i];
            for(const auto &e : g_[i]) {
                int v = cmp_[e.to];
                if(u == v || st.count({u, v}))
                    continue;
                st.insert({u, v});
                ret.add_directed_edge(u, v, e.cost);
            }
        }
        return ret;
    }

  private:
    void dfs(int v) {
        used_[v] = true;
        for(const auto &e : g_[v]) {
            if(!used_[e.to])
                dfs(e.to);
        }
        vs_.push_back(v);
    }

    void rdfs(int v, int k) {
        used_[v] = false;
        cmp_[v] = k;
        for(const auto &e : rg_[v]) {
            if(used_[e.to])
                rdfs(e.to, k);
        }
    }
};

} // namespace momohara
