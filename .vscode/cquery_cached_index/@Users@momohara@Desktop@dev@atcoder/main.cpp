#include <bits/stdc++.h>
using namespace std;

using ll=long long;
template <class T> using vec=vector<T>;
template <class T> using vvec=vec<vec<T>>;

#define rep(i,n,m) for(ll (i)=(n);(i)<(m);i++)
#define REP(i,n) rep(i,0,n)
#define en "\n"

constexpr ll MOD = (ll)1e9+7;

int main(){
    ll n,m,L,R;
    cin>>n>>m>>L>>R;

    vec<ll> sum(m+1,0);
    REP(i,m) sum[i+1] = (sum[i]+m-i+MOD)%MOD;

    ll ans=0;

    REP(i,n){
        if(i>R) break;
        ll l1=-1;
        ll r1=m;
        while(l1+1<r1){
            ll m=l1+r1>>1;
            if(i*i+m*m>=L*L) r1=m;
            else l1=m;
        }
        if(i*i+r1*r1<L*L) continue;
        
        ll l2=0;
        ll r2=m;
        while(l2+1<r2){
            ll m=l2+r2>>1;
            if(i*i+m*m<=R*R) l2=m;
            else r2=m;
        }
        if(i*i+l2*l2>R*R) continue;
        cout<<l2<<" "<<r1<<en;
        (ans += (n-i)*(MOD + sum[l2+1]-sum[r1])%MOD)%=MOD;
        cout<<(n-i)<<" "<<(MOD + sum[l2+1]-sum[r1])%MOD<<"\n";
    }

    cout<<ans<<"\n";

    return 0;
}