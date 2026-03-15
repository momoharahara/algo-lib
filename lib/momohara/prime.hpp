#pragma once

#include <vector>

namespace momohara {

inline std::vector<int> prime(long long n) {
    std::vector<int> arr(n + 1, 1);
    if(n >= 0)
        arr[0] = 0;
    if(n >= 1)
        arr[1] = 0;
    for(long long i = 2; i * 2 <= n; ++i) {
        if(arr[i]) {
            for(long long j = 2; i * j <= n; ++j) {
                arr[i * j] = 0;
            }
        }
    }
    return arr;
}

} // namespace momohara
