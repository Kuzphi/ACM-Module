#include <cmath>
#include <time.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#define ll long long
using namespace std;
ll Mul(ll a,ll b ,ll Mo){
    ll ans = 0;
    for (;b;b>>=1,(a<<=1)%=Mo)
        if(b&1) ans = (ans + a) % Mo;
    return ans;
}
ll Pow(ll a,ll b ,ll Mo){
    ll ans = 1;
    for (;b;b>>=1,a=Mul(a,a,Mo))
        if(b&1) ans = Mul(ans,a,Mo);
    return ans;
}
struct Miller_Rabin{
    bool _Miller_Rabin(ll a ,ll N){
        ll cnt = 0, p = N-1;
        while(!(p&1)) cnt++, p>>=1;
        a = Pow(a,p,N);
        while(cnt -- ){
            ll t = Mul(a,a,N);
            if (t==1 && a!=1 && a!=N-1) return 0;
            a = t;
        }
        if (a - 1) return 0;
        return 1;
    }
    bool Check(ll N){
        int cs = 10;
        if (N<3) return N==2;
        while(cs--)
            if (! _Miller_Rabin( rand()% (N-2)+2 , N)) return 0;
        return 1;
    }
}MR;
struct Pollard_Rho{
    ll _Pollard_Rho(ll N,ll C){
        ll x = rand()%N;
        for (ll i = 2,y = x,k = 2;1;i++){
            x = ( Mul(x,x,N) + C) %N;
            ll d = __gcd( abs(y-x) , N );
            if (1 < d && d < N) return d;
            if (x == y) return N;
            if (i == k) y = x, k<<=1;
        }
    }
    void Solve(ll x,vector<int>& A){        
        if (x == 1) return;
        if (MR.Check(x)) {A.push_back(x); return;}
        ll d = x;
        while(d==x) d = _Pollard_Rho(x,rand()%(x-1)+1);
        Solve(d,A);Solve(x/d,A);
    } 
}PR;