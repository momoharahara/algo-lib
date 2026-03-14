#include <bits/stdc++.h>

using namespace std;

using ll = long long;
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
#define rep(i, m, n) for(ll i = (ll)(m); i < (ll)(n); ++i)
#define rep2(i, m, n) for(ll i = (ll)(n) - 1; i >= (ll)(m); --i)
#define REP(i, n) rep(i, 0, n)
#define REP2(i, n) rep2(i, 0, n)

constexpr long long INF = 1LL << 60;
constexpr int INF_INT = 1 << 30;
constexpr long long MOD = (ll)1e9 + 7;
// constexpr long long MOD = 998244353LL;
static const ld pi = 3.141592653589793L;

#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

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

void solve(ll T) {
    ll ans = 0;
    cout << "Case #" << T << ": ";
    cout << ans << en;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    ll t;
    cin >> t;
    rep(i, 1, t + 1) {
        solve(i);
    }

    return 0;
}
