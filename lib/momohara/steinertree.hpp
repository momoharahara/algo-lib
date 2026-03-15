#pragma once

#include <functional>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

#include "graph.hpp"

namespace momohara {

template <typename T>
class SteinerTree {
    int n_;
    Graph<T> g_;
    T inf_;

  public:
    std::vector<std::vector<T>> dp;

    explicit SteinerTree(const Graph<T> &g) : n_(static_cast<int>(g.size())), g_(g), inf_(std::numeric_limits<T>::max() / 10) {}

    std::vector<T> solve(std::vector<int> &terminal) {
        int t = static_cast<int>(terminal.size());
        if(t == 0)
            return {};
        int all = 1 << t;
        dp.assign(all, std::vector<T>(n_, inf_));
        for(int i = 0; i < t; ++i)
            dp[1 << i][terminal[i]] = 0;
        using pti = std::pair<T, int>;
        for(int bit = 1; bit < all; ++bit) {
            for(int k = bit; k > 0; k = (k - 1) & bit) {
                for(int v = 0; v < n_; ++v)
                    dp[bit][v] = std::min(dp[bit][v], dp[k][v] + dp[bit ^ k][v]);
            }
            std::priority_queue<pti, std::vector<pti>, std::greater<>> que;
            for(int v = 0; v < n_; ++v)
                que.emplace(dp[bit][v], v);
            while(!que.empty()) {
                auto [d, v] = que.top();
                que.pop();
                if(dp[bit][v] < d)
                    continue;
                for(const auto &e : g_[v]) {
                    if(dp[bit][e.to] > dp[bit][v] + e.cost) {
                        dp[bit][e.to] = dp[bit][v] + e.cost;
                        que.emplace(dp[bit][e.to], e.to);
                    }
                }
            }
        }
        return dp[all - 1];
    }
};

} // namespace momohara
