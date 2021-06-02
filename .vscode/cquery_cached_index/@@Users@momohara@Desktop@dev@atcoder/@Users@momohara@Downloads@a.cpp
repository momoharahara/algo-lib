/*
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
//*/
 
#include <bits/stdc++.h>
 
// #include <atcoder/all>
 
using namespace std;
// using namespace atcoder;
 
#define DEBUG(x) cerr<<#x<<": "<<x<<endl;
#define DEBUG_VEC(v) cerr<<#v<<":";for(int i=0;i<v.size();i++) cerr<<" "<<v[i]; cerr<<endl;
#define DEBUG_MAT(v) cerr<<#v<<endl;for(int i=0;i<v.size();i++){for(int j=0;j<v[i].size();j++) {cerr<<v[i][j]<<" ";}cerr<<endl;}
typedef long long ll;
#define int ll
 
#define vi vector<int>
#define vl vector<ll>
#define vii vector< vector<int> >
#define vll vector< vector<ll> >
#define vs vector<string>
#define pii pair<int,int>
#define pis pair<int,string>
#define psi pair<string,int>
#define pll pair<ll,ll>
template<class S, class T> pair<S, T> operator+(const pair<S, T> &s, const pair<S, T> &t) { return pair<S, T>(s.first + t.first, s.second + t.second); }
template<class S, class T> pair<S, T> operator-(const pair<S, T> &s, const pair<S, T> &t) { return pair<S, T>(s.first - t.first, s.second - t.second); }
template<class S, class T> ostream& operator<<(ostream& os, pair<S, T> p) { os << "(" << p.first << ", " << p.second << ")"; return os; }
#define X first
#define Y second
#define rep(i,n) for(int i=0;i<(int)(n);i++)
#define rep1(i,n) for(int i=1;i<=(int)(n);i++)
#define rrep(i,n) for(int i=(int)(n)-1;i>=0;i--)
#define rrep1(i,n) for(int i=(int)(n);i>0;i--)
#define REP(i,a,b) for(int i=a;i<b;i++)
#define in(x, a, b) (a <= x && x < b)
#define all(c) c.begin(),c.end()
void YES(bool t=true) {cout<<(t?"YES":"NO")<<endl;}
void Yes(bool t=true) {cout<<(t?"Yes":"No")<<endl;}
void yes(bool t=true) {cout<<(t?"yes":"no")<<endl;}
void NO(bool t=true) {cout<<(t?"NO":"YES")<<endl;}
void No(bool t=true) {cout<<(t?"No":"Yes")<<endl;}
void no(bool t=true) {cout<<(t?"no":"yes")<<endl;}
template<class T> bool chmax(T &a, const T &b) { if (a<b) { a = b; return 1; } return 0; }
template<class T> bool chmin(T &a, const T &b) { if (a>b) { a = b; return 1; } return 0; }
#define UNIQUE(v) v.erase(std::unique(v.begin(), v.end()), v.end());
const ll inf = 1000000001;
const ll INF = (ll)1e18 + 1;
const long double pi = 3.1415926535897932384626433832795028841971L;
int popcount(ll t) { return __builtin_popcountll(t); }
// int dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1};
// int dx2[8] = { 1,1,0,-1,-1,-1,0,1 }, dy2[8] = { 0,1,1,1,0,-1,-1,-1 };
vi dx = {0, 1, 0, -1}, dy = {-1, 0, 1, 0};
// vi dx2 = { 1,1,0,-1,-1,-1,0,1 }, dy2 = { 0,1,1,1,0,-1,-1,-1 };
struct Setup_io {
    Setup_io() {
        ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
        cout << fixed << setprecision(25);
    }
} setup_io;
// const ll MOD = 1000000007;
const ll MOD = 998244353;
// #define mp make_pair
//#define endl '\n'


void solve() {
    int n, m;
    cin >> n >> m;
    vi a(n), b(m);
    rep (i, n) cin >> a[i];
    rep (i, m) cin >> b[i];

    sort(all(a));
    sort(all(b));
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    rep (i, n) {
        pq.push(pii(a[i], i));
    }

    vi val(n);
    vii group(n);
    rep (i, n) {
        val[i] = a[i];
        group[i].push_back(i);
    }
    
    int use_mi = inf;
    int suc = 0;
    rep (i, m) {
        while (pq.size() > 0) {
            pii x = pq.top();
            pq.pop();
            if (x.first >= b[i]) {
                suc++;
                int idx = x.second;
                for (int u: group[idx]) chmin(use_mi, a[u]);
                break;
            }
            if (pq.size() == 0) break;
            pii y = pq.top();
            pq.pop();
            if (y.first >= b[i]) {
                suc++;
                int idx = y.second;
                for (int u: group[idx]) chmin(use_mi, a[u]);
                pq.push(x);
                
                break;
            }

            int mi = min(x.first, y.first), ma = max(x.first, y.first);
            int z;
            if (mi == ma) {
                z = 2 * mi;
            }
            else {
                z = 2 * mi + 1;
            }
            int u = x.second, v = y.second;
            if (group[u].size() > group[v].size()) swap(u, v);
            for (int idx: group[u]) {
                group[v].push_back(idx);
            }
            pq.push(pii(z, v));
        }
    }

    while (pq.size() > 1) {
        pii x = pq.top();
        pq.pop();
        pii y = pq.top();
        pq.pop();
        int mi = min(x.first, y.first), ma = max(x.first, y.first);
        int z;
        if (mi == ma) {
            z = 2 * mi;
        }
        else {
            z = 2 * mi + 1;
        }
        pq.push(pii(z, -1));
    }
    int las;
    if (pq.size() == 0) las = INF;
    else las = pq.top().first;

    if (suc == m and las >= use_mi - 1) Yes();
    else No();
}

signed main() {
    int t;
    cin >> t;
    while (t--) solve();
}
