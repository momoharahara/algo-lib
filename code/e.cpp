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
    ll h, w;
    cin >> h >> w;
    vec<string> s(h);
    REP(i, h) {
        cin >> s[i];
    }

    vvec<ll> sumxy(h + w + 1, vec<ll>(h + w + 1, 0));
    vvec<ll> sumyx(h + w + 1, vec<ll>(h + w + 1, 0));
    vvec<ll> xy(h + w + 1, vec<ll>(h + w + 1, 0));
    vvec<ll> yx(h + w + 1, vec<ll>(h + w + 1, 0));
    vvec<ll> xys(h + w + 1);
    vvec<ll> yxs(h + w + 1);

    REP(i, h) {
        REP(j, w) {
            if(s[i][j] == '#') {
                xy[i + j][i - j + w]++;
                yx[i - j + w][i + j]++;
                yxs[i - j + w].push_back({i + j});
            }
        }
    }

    REP(i, h) {
        REP2(j, w) {
            if(s[i][j] == '#') {
                xys[i + j].push_back({i - j + w});
            }
        }
    }

    REP(i, h + w + 1) {
        REP(j, h + w) {
            sumyx[i][j + 1] += sumyx[i][j] + yx[i][j];
            sumxy[i][j + 1] += sumxy[i][j] + xy[i][j];
        }
    }

    ll ans = 0;
    REP(i, h + w + 1) {
        REP(j, yxs[i].size()) {
            rep(k, j + 1, yxs[i].size()) {
                ll d = yxs[i][k] - yxs[i][j];
                if(i + d <= h + w) {
                    ans += sumyx[i + d][yxs[i][k]] - sumyx[i + d][yxs[i][j] + 1];
                }
                if(i - d >= 0) {
                    ans += sumyx[i - d][yxs[i][k]] - sumyx[i - d][yxs[i][j] + 1];
                }
            }
        }
    }

    REP(i, h + w + 1) {
        REP(j, xys[i].size()) {
            rep(k, j + 1, xys[i].size()) {
                ll d = xys[i][k] - xys[i][j];
                if(i + d <= h + w) {
                    ans += sumxy[i + d][xys[i][k] + 1] - sumxy[i + d][xys[i][j]];
                }
                if(i - d >= 0) {
                    ans += sumxy[i - d][xys[i][k] + 1] - sumxy[i - d][xys[i][j]];
                }
            }
        }
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
