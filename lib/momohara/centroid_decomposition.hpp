#pragma once

#include <vector>

namespace momohara {

struct CentroidDecomposition {
    std::vector<int> rank;
    std::vector<int> par;
    std::vector<std::vector<int>> child;

  private:
    int n;
    std::vector<std::vector<int>> g;
    std::vector<int> sz;

    void calc_size(int v, int p) {
        sz[v] = 1;
        for(int to : g[v]) {
            if(rank[to] == -1 && to != p) {
                calc_size(to, v);
                sz[v] += sz[to];
            }
        }
    }

    void build_dfs(int v, int p) {
        calc_size(v, -1);
        int tot = sz[v];
        bool ok = false;
        int pp = -1;
        while(!ok) {
            ok = true;
            for(int to : g[v]) {
                if(rank[to] == -1 && to != pp && 2 * sz[to] > tot) {
                    pp = v;
                    v = to;
                    ok = false;
                    break;
                }
            }
        }
        par[v] = p;
        if(p != -1) {
            child[p].push_back(v);
            rank[v] = rank[p] + 1;
        } else {
            rank[v] = 0;
        }
        for(int to : g[v]) {
            if(rank[to] == -1)
                build_dfs(to, v);
        }
    }

  public:
    explicit CentroidDecomposition(const std::vector<std::vector<int>> &g)
        : rank(g.size(), -1), par(g.size(), -1), child(g.size()), n(static_cast<int>(g.size())), g(g), sz(g.size()) {}

    void build() {
        if(n > 0)
            build_dfs(0, -1);
    }
};

} // namespace momohara
