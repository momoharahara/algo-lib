#pragma once

#include <algorithm>
#include <climits>
#include <vector>


inline std::vector<long long> lis_fast(const std::vector<long long> &a) {
    long long n = static_cast<long long>(a.size());
    std::vector<long long> best(n, INT_MAX);
    std::vector<long long> id(n);
    for(int i = 0; i < n; ++i) {
        id[i] = std::distance(best.begin(), std::lower_bound(best.begin(), best.end(), a[i]));
        best[id[i]] = a[i];
    }
    long long m = *std::max_element(id.begin(), id.end());
    std::vector<long long> b(m + 1);
    for(int i = static_cast<int>(n) - 1; i >= 0; --i) {
        if(id[i] == m)
            b[m--] = a[i];
    }
    return b;
}

