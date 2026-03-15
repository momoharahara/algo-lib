#pragma once

#include <functional>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

namespace momohara {

template <typename W>
struct RangeEdgeGraph {
    struct Edge {
        int to;
        W weight;
    };

    int n;
    std::vector<std::vector<Edge>> g;

    explicit RangeEdgeGraph(int n) : n(n), g(4 * n) {
        for(int i = 1; i < n; ++i) {
            int cl = i << 1;
            int cr = i << 1 | 1;
            g[i].push_back({cl, 0});
            g[i].push_back({cr, 0});
            g[cl + 2 * n].push_back({i + 2 * n, 0});
            g[cr + 2 * n].push_back({i + 2 * n, 0});
        }
        for(int i = n; i < 2 * n; ++i)
            g[i].push_back({i + 2 * n, 0});
    }

    void add_edge(int l1, int r1, int l2, int r2, W w) {
        int idx = static_cast<int>(g.size());
        for(l1 += n, r1 += n; l1 < r1; l1 >>= 1, r1 >>= 1) {
            if(l1 & 1)
                g[l1 + 2 * n].push_back({idx, 0}), ++l1;
            if(r1 & 1)
                --r1, g[r1 + 2 * n].push_back({idx, 0});
        }
        std::vector<Edge> node;
        for(l2 += n, r2 += n; l2 < r2; l2 >>= 1, r2 >>= 1) {
            if(l2 & 1)
                node.push_back({l2++, w});
            if(r2 & 1)
                node.push_back({--r2, w});
        }
        g.push_back(node);
    }

    std::vector<W> dijkstra(int s) const {
        int start = s + n;
        std::vector<W> dist(g.size(), std::numeric_limits<W>::max());
        dist[start] = 0;
        using P = std::pair<W, int>;
        std::priority_queue<P, std::vector<P>, std::greater<>> que;
        que.emplace(0, start);
        while(!que.empty()) {
            auto [dd, v] = que.top();
            que.pop();
            if(dist[v] < dd)
                continue;
            for(const auto &e : g[v]) {
                if(dist[e.to] > dist[v] + e.weight) {
                    dist[e.to] = dist[v] + e.weight;
                    que.emplace(dist[e.to], e.to);
                }
            }
        }
        return std::vector<W>(dist.begin() + n, dist.begin() + 2 * n);
    }
};

} // namespace momohara
