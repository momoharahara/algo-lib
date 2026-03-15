#pragma once

#include <vector>

#include "graph.hpp"

namespace momohara {

template <typename E, typename F, typename PE, typename PV, typename T = int>
class RerootingDP {
    int n_;
    E id_;
    F merge_;
    PE put_edge_;
    PV put_vertex_;
    Graph<T> g_;
    std::vector<E> subdp_;
    std::vector<E> dp_;

    void build_impl(int root) {
        auto dfs = [&](auto &&self, int v, int p) -> void {
            E val = id_;
            for(const auto &e : g_[v]) {
                if(e.to == p)
                    continue;
                self(self, e.to, v);
                val = merge_(val, put_edge_(subdp_[e.to], e));
            }
            subdp_[v] = put_vertex_(val, v);
        };
        dfs(dfs, root, -1);
    }

    void reroot_impl(int root) {
        auto dfs = [&](auto &&self, int v, int p, E pval) -> void {
            std::vector<E> as;
            for(const auto &e : g_[v]) {
                if(e.to == p) {
                    as.push_back(put_edge_(pval, e));
                } else {
                    as.push_back(put_edge_(subdp_[e.to], e));
                }
            }
            std::vector<E> al(as.size()), ar(as.size());
            if(!as.empty()) {
                al[0] = as[0];
                for(int i = 0; i + 1 < static_cast<int>(as.size()); ++i)
                    al[i + 1] = merge_(as[i + 1], al[i]);
                ar.back() = as.back();
                for(int i = static_cast<int>(as.size()) - 2; i >= 0; --i)
                    ar[i] = merge_(as[i], ar[i + 1]);
                dp_[v] = merge_(dp_[v], put_vertex_(al.back(), v));
            } else {
                dp_[v] = merge_(dp_[v], put_vertex_(id_, v));
            }
            ans[v] = dp_[v];
            int k = 0;
            for(const auto &e : g_[v]) {
                if(e.to == p) {
                    ++k;
                    continue;
                }
                E val = id_;
                if(k - 1 >= 0)
                    val = merge_(val, al[k - 1]);
                if(k + 1 < static_cast<int>(ar.size()))
                    val = merge_(val, ar[k + 1]);
                val = put_vertex_(val, v);
                self(self, e.to, v, val);
                ++k;
            }
        };
        dfs(dfs, root, -1, id_);
    }

  public:
    std::vector<E> ans;

    RerootingDP(const Graph<T> &g, E id, F merge, PE put_edge, PV put_vertex, int root = 0)
        : n_(static_cast<int>(g.size())),
          id_(id),
          merge_(merge),
          put_edge_(put_edge),
          put_vertex_(put_vertex),
          g_(g),
          subdp_(n_, id),
          dp_(n_, id),
          ans(n_, id) {
        build_impl(root);
        reroot_impl(root);
    }
};

} // namespace momohara
