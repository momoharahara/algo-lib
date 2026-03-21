#pragma once

#include <utility>
#include <vector>


template <typename T>
struct RollingHash {
    static constexpr int base1 = 1007;
    static constexpr int base2 = 2009;
    static constexpr int mod1 = 1000000007;
    static constexpr int mod2 = 1000000009;

    std::vector<long long> hash1;
    std::vector<long long> hash2;
    std::vector<long long> power1;
    std::vector<long long> power2;

    explicit RollingHash(const T &s) {
        int n = static_cast<int>(s.size());
        hash1.assign(n + 1, 0);
        hash2.assign(n + 1, 0);
        power1.assign(n + 1, 1);
        power2.assign(n + 1, 1);
        for(int i = 0; i < n; ++i) {
            hash1[i + 1] = (hash1[i] * base1 + s[i]) % mod1;
            hash2[i + 1] = (hash2[i] * base2 + s[i]) % mod2;
            power1[i + 1] = (power1[i] * base1) % mod1;
            power2[i + 1] = (power2[i] * base2) % mod2;
        }
    }

    std::pair<long long, long long> get(int l, int r) const {
        long long res1 = hash1[r] - hash1[l] * power1[r - l] % mod1;
        if(res1 < 0)
            res1 += mod1;
        long long res2 = hash2[r] - hash2[l] * power2[r - l] % mod2;
        if(res2 < 0)
            res2 += mod2;
        return {res1, res2};
    }
};

