#include <momohara/macro.hpp>

#include <atcoder/all>
using namespace atcoder;

template <int m>
ostream &operator<<(ostream &os, const static_modint<m> &x) {
    return os << x.val();
}

ostream &operator<<(ostream &os, const modint &x) {
    return os << x.val();
}

void solve() {
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    // cout << fixed << setprecision(10);

    // ll t;
    // cin >> t;
    // rep(i, t - 1) {
    //     solve();
    // }

    solve();

    return 0;
}
