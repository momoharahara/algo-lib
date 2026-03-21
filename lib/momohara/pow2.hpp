#pragma once


inline long long pow2(long long x, long long n) {
    long long ans = 1;
    while(n > 0) {
        if(n & 1)
            ans *= x;
        x *= x;
        n >>= 1;
    }
    return ans;
}

inline long long pow2big(long long x, long long n, long long ma) {
    long long ans = 1;
    bool overflow = false;
    while(n > 0) {
        if(n & 1) {
            if(overflow || ans > ma / x)
                return ma + 1;
            ans *= x;
        }
        if(x > ma / x) {
            overflow = true;
        } else {
            x *= x;
        }
        n >>= 1;
    }
    return ans;
}

inline long long pow2_m(long long x, long long n, long long mod) {
    long long ans = 1;
    while(n > 0) {
        if(n & 1)
            ans = ans * x % mod;
        x = x * x % mod;
        n >>= 1;
    }
    return ans;
}

