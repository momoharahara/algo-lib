#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include "graph.hpp"

namespace momohara {

template <typename T>
long long MaxIndependentSet(const Graph<T> &g) {
    int n = static_cast<int>(g.size());
    long long all = (1LL << n) - 1;
    std::vector<long long> ok(n, all), ng(n, 0);
    for(int v = 0; v < n; ++v) {
        for(const auto &e : g[v]) {
            ok[v] ^= (1LL << e.to);
            ng[v] |= (1LL << e.to);
        }
    }

    int l = n / 2;
    int r = n - l;
    long long left = 1LL << l;
    std::vector<long long> dp(left, 0);
    for(long long bit = 0; bit < left; ++bit) {
        bool flag = true;
        for(int i = 0; i < l; ++i) {
            if((bit >> i) & 1) {
                dp[bit] = std::max(dp[bit], dp[bit ^ (1LL << i)]);
                if(bit & ng[i])
                    flag = false;
            }
        }
        if(flag)
            dp[bit] = std::max(dp[bit], static_cast<long long>(__builtin_popcountll(bit)));
    }

    long long right = 1LL << r;
    long long ret = 0;
    for(long long bit = 0; bit < right; ++bit) {
        bool flag = true;
        long long mask = left - 1;
        for(int i = 0; i < r; ++i) {
            if((bit >> i) & 1) {
                mask &= ok[i + l];
                if(bit & (ng[i + l] >> l))
                    flag = false;
            }
        }
        if(flag)
            ret = std::max(ret, dp[mask] + static_cast<long long>(__builtin_popcountll(bit)));
    }
    return ret;
}

} // namespace momohara
