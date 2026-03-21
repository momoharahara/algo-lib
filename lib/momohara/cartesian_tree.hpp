#pragma once

#include <algorithm>
#include <utility>
#include <vector>


template <typename T, typename F>
class CartesianTree {
    int n_;
    std::vector<T> a_;
    int root_;
    F op_;
    std::vector<std::vector<int>> g_;
    std::vector<int> l_;
    std::vector<int> r_;

  public:
    CartesianTree(F op, std::vector<T> a) : n_(static_cast<int>(a.size())), a_(std::move(a)), root_(-1), op_(op), g_(n_), l_(n_), r_(n_) {
        std::vector<int> p(n_, -1);
        std::vector<int> st;
        st.reserve(n_);
        for(int i = 0; i < n_; ++i) {
            int prv = -1;
            while(!st.empty() && op_(a_[i], a_[st.back()])) {
                prv = st.back();
                st.pop_back();
            }
            if(prv != -1)
                p[prv] = i;
            if(!st.empty())
                p[i] = st.back();
            st.push_back(i);
        }
        for(int i = 0; i < n_; ++i) {
            if(p[i] != -1) {
                g_[p[i]].push_back(i);
            } else {
                root_ = i;
            }
        }
        calc();
    }

    int root() const {
        return root_;
    }

    const std::vector<std::vector<int>> &graph() const {
        return g_;
    }

    std::pair<int, int> get_range(int v) const {
        return {l_[v], r_[v]};
    }

  private:
    void calc() {
        for(int i = 0; i < n_; ++i) {
            l_[i] = i;
            r_[i] = i + 1;
        }
        auto dfs = [&](auto &&self, int v) -> void {
            for(int to : g_[v]) {
                self(self, to);
                l_[v] = std::min(l_[v], l_[to]);
                r_[v] = std::max(r_[v], r_[to]);
            }
        };
        if(root_ != -1)
            dfs(dfs, root_);
    }
};

