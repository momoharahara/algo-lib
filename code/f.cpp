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
#define rep2(i, m, n) for(ll i = (ll)(n)-1; i >= (ll)(m); --i)
#define REP(i, n) rep(i, 0, n)
#define REP2(i, n) rep2(i, 0, n)

constexpr long long INF = 1LL << 60;
constexpr int INF_INT = 1 << 25;
//constexpr long long MOD = (ll)1e9 + 7;
constexpr long long MOD = 998244353LL;
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

//グラフ関連
struct Edge {
    int to, rev;
    ll cap;
    Edge(int _to, ll _cap, int _rev) : to(_to), cap(_cap), rev(_rev) {}
};

typedef vector<Edge> Edges;
typedef vector<Edges> Graph;

void add_edge(Graph &G, int from, int to, ll cap, bool revFlag, ll revCap) {
    G[from].push_back(Edge(to, cap, (int)G[to].size()));
    if(revFlag)
        G[to].push_back(Edge(from, revCap, (int)G[from].size() - 1));
}
template <int mod>
struct ModInt {
    int x;

    ModInt() : x(0) {}

    ModInt(int64_t y) : x(y >= 0 ? y % mod : (mod - (-y) % mod) % mod) {}

    ModInt &operator+=(const ModInt &p) {
        if((x += p.x) >= mod)
            x -= mod;
        return *this;
    }

    ModInt &operator-=(const ModInt &p) {
        if((x += mod - p.x) >= mod)
            x -= mod;
        return *this;
    }

    ModInt &operator*=(const ModInt &p) {
        x = (int)(1LL * x * p.x % mod);
        return *this;
    }

    ModInt &operator/=(const ModInt &p) {
        *this *= p.inverse();
        return *this;
    }

    ModInt operator-() const { return ModInt(-x); }

    ModInt operator+(const ModInt &p) const { return ModInt(*this) += p; }

    ModInt operator-(const ModInt &p) const { return ModInt(*this) -= p; }

    ModInt operator*(const ModInt &p) const { return ModInt(*this) *= p; }

    ModInt operator/(const ModInt &p) const { return ModInt(*this) /= p; }

    bool operator==(const ModInt &p) const { return x == p.x; }

    bool operator!=(const ModInt &p) const { return x != p.x; }

    ModInt inverse() const {
        int a = x, b = mod, u = 1, v = 0, t;
        while(b > 0) {
            t = a / b;
            swap(a -= t * b, b);
            swap(u -= t * v, v);
        }
        return ModInt(u);
    }

    ModInt pow(int64_t n) const {
        ModInt ret(1), mul(x);
        while(n > 0) {
            if(n & 1)
                ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }

    friend ostream &operator<<(ostream &os, const ModInt &p) {
        return os << p.x;
    }

    friend istream &operator>>(istream &is, ModInt &a) {
        int64_t t;
        is >> t;
        a = ModInt<mod>(t);
        return (is);
    }

    static int get_mod() { return mod; }
};

using mint = ModInt<MOD>;

void solve() {
    string s;
    cin >> s;
    ll n = s.size();

    vec<int> blue(n * 2 + 1, 0);
    REP(i, n) {
        blue[i + 1] = blue[i] + s[i] - '0';
    }
    rep(i, n, n * 2) {
        blue[i + 1] = blue[i];
    }

    vvec<mint> dp(n * 2 + 1, vec<mint>(n * 2 + 1, 0));
    dp[0][0] = 1;
    REP(i, n * 2) {
        REP(b, blue[i] + 1) {
            int red = min(n * 2, (i + 1) * 2) - blue[i + 1];
            int r = i - b;
            if(b + 1 <= blue[i + 1])
                dp[i + 1][b + 1] += dp[i][b];
            if(r + 1 <= red)
                dp[i + 1][b] += dp[i][b];
        }
    }

    cout << dp[n * 2][blue[n]] << en;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    // ll t;
    // cin >> t;
    // REP(i, t - 1) {
    //     solve();
    // }

    solve();

    return 0;
}
