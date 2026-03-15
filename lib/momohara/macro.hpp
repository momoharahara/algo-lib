#pragma once

#include <bits/stdc++.h>

using namespace std;

#ifdef DEBUG
#include "debug/all.hpp"
#else
#define debug(...) 42
#endif

using ll = long long;
using i128 = __int128_t;
using ull = unsigned long long;
using ld = long double;
using P = pair<ll, ll>;
using tp = tuple<ll, ll, ll>;

template <class T>
using vec = vector<T>;
template <class T>
using vvec = vector<vec<T>>;

#define all(hoge) (hoge).begin(), (hoge).end()
#define en '\n'
#define REP(i, m, n) for(ll i = (ll)(m); i < (ll)(n); ++i)
#define REP2(i, m, n) for(ll i = (ll)(n) - 1; i >= (ll)(m); --i)
#define rep(i, n) REP(i, 0, n)
#define rep2(i, n) REP2(i, 0, n)

constexpr long long INF = 1LL << 60;
constexpr int INF_INT = 1 << 30;
// constexpr long long MOD = (ll)1e9 + 7;
constexpr long long MOD = 998244353LL;
static const ld pi = 3.141592653589793L;

template <class T>
inline bool chmin(T &a, T b) {
    if(a > b) {
        a = b;
        return true;
    }
    return false;
}

template <class T>
inline bool chmax(T &a, T b) {
    if(a < b) {
        a = b;
        return true;
    }
    return false;
}