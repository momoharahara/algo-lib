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
constexpr long long MOD = (ll)1e9 + 7;
//constexpr long long MOD = 998244353LL;
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
    Edge(int _to, int _rev, ll _cap) : to(_to), rev(_rev), cap(_cap) {}
};

typedef vector<Edge> Edges;
typedef vector<Edges> Graph;

void add_edge(Graph &G, int from, int to, ll cap, bool revFlag, ll revCap) {
    G[from].push_back(Edge(to, (int)G[to].size(), cap));
    if(revFlag)
        G[to].push_back(Edge(from, (int)G[from].size() - 1, revCap));
}

void solve() {
    ll n;
    cin >> n;
    Graph g(n);
    REP(i, n - 1) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        add_edge(g, a, b, 1, true, 1);
    }

    vec<ll> size(n);
    auto dfs = [&](auto &&self, int v, int p) -> void {
        size[v] = 1;
        for(auto e : g[v]) {
            if(e.to == p)
                continue;
            self(self, e.to, v);
            size[v] += size[e.to];
        }
    };

    dfs(dfs, 0, -1);
    ll ans = 0;
    rep(i, 1, n) {
        ans += (n - size[i]) * size[i];
    }
    cout << ans << en;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    //cout << fixed << setprecision(10);

    // ll t;
    // cin >> t;
    // REP(i, t - 1) {
    //     solve();
    // }

    solve();

    return 0;
}
