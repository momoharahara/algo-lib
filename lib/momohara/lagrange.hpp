#pragma once

#include <vector>

namespace momohara {

template <class T>
T LagrangeInterpolation(const std::vector<T> &y, long long t) {
    int n = static_cast<int>(y.size()) - 1;
    if(t <= n)
        return y[t];
    T fi = 1;
    T ft = 1;
    for(long long i = 0; i <= n; ++i) {
        if(i)
            fi *= -i;
        ft *= (t - i);
    }
    T ret = 0;
    for(long long i = 0; i <= n; ++i) {
        ret += y[i] * ft / (t - i) / fi;
        fi *= (i + 1);
        fi /= (i - n);
    }
    return ret;
}

} // namespace momohara
