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

template <typename T, typename E, typename F, typename G, typename H, typename P>
class LazySegmentTree {
  private:
    const T identity_t;
    const E identity_e;
    F f_tt;
    G f_te;
    H f_ee;
    P f_ep; //区間に比例する場合
    vector<T> dat;
    vector<E> lazy;
    int n;
    void eval(int k, int len) {
        if(lazy[k] == identity_e)
            return;
        if(k < n - 1) {
            lazy[2 * k + 1] = f_ee(lazy[2 * k + 1], lazy[k]);
            lazy[2 * k + 2] = f_ee(lazy[2 * k + 2], lazy[k]);
        }
        dat[k] = f_te(dat[k], f_ep(lazy[k], len));
        lazy[k] = identity_e;
    }
    void update(int a, int b, E x, int k, int l, int r) {
        eval(k, r - l);
        if(a <= l && r <= b) {
            lazy[k] = f_ee(lazy[k], x);
            eval(k, r - l);
        } else if(a < r && l < b) {
            update(a, b, x, k * 2 + 1, l, (l + r) / 2);
            update(a, b, x, k * 2 + 2, (l + r) / 2, r);
            dat[k] = f_tt(dat[2 * k + 1], dat[2 * k + 2]);
        }
    }
    T query(int a, int b, int k, int l, int r) {
        eval(k, r - l);
        if(r <= a || b <= l)
            return identity_t;
        else if(a <= l && r <= b)
            return dat[k];
        else {
            T vl = query(a, b, 2 * k + 1, l, (l + r) / 2);
            T vr = query(a, b, 2 * k + 2, (l + r) / 2, r);
            return f_tt(vl, vr);
        }
    }

  public:
    LazySegmentTree(vector<T> v, T id_t, E id_e, F f, G g, H h, P p) : identity_t(id_t), identity_e(id_e), f_tt(f), f_te(g), f_ee(h), f_ep(p) {
        n = 1;
        while(n < v.size())
            n *= 2;
        dat.resize(2 * n - 1, identity_t);
        lazy.resize(2 * n - 1, identity_e);
        REP(i, v.size())
        dat[n + i - 1] = v[i];
        for(int i = n - 2; i >= 0; i--)
            dat[i] = f_tt(dat[i * 2 + 1], dat[i * 2 + 2]);
    }
    void update(int a, int b, E x) { update(a, b, x, 0, 0, n); }
    T query(int a, int b) { return query(a, b, 0, 0, n); }
};

void solve() {
    ll w, n;
    cin >> w >> n;
    vec<ll> l(n), r(n), v(n);
    REP(i, n) {
        cin >> l[i] >> r[i] >> v[i];
        r[i]++;
    }

    //*一次関数(区間 max、区間max)
    using S = ll;
    using L = ll;
    S id_t = -INF;
    L id_e = -INF;
    auto op = [](S a, S b) { return max(a, b); };          //dat->dat
    auto mapping = [](S a, L b) { return max(a, b); };     //lazy->dat
    auto composition = [](L a, L b) { return max(a, b); }; //lazy->lazy
    auto p = [](L a, ll b) { return a; };                  //lazyが区間に比例するか
    vector seg(2, LazySegmentTree(vec<S>(w + 1, id_t), id_t, id_e, op, mapping, composition, p));
    seg[0].update(0, 1, 0);

    REP(i, n) {
        int pre = i % 2;
        int nxt = !pre;
        REP(j, w + 1) {
            ll val = seg[pre].query(j, j + 1);
            if(val == -INF)
                continue;
            seg[nxt].update(j, j + 1, val);
            ll l2 = j + l[i];
            ll r2 = min(w + 1, j + r[i]);
            if(l2 < r2)
                seg[nxt].update(l2, r2, val + v[i]);
        }
    }

    ll ans = seg[n % 2].query(w, w + 1);

    if(ans == -INF)
        cout << -1 << en;
    else
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
