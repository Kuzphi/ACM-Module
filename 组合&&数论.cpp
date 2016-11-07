#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <string.h>
#include <algorithm>
#define Maxn 10010
#define CLR(x) memset(x,0,sizeof(x))
#define ll long long
using namespace std;
const int oo=2100000000;
ll Mo;
ll m[Maxn],p[Maxn],c[Maxn],res[Maxn];
int len,n,k;


ll Ex_Gcd(ll a,ll b,ll &x,ll&y)//扩展GCD ax+by==1 返回gcd(a,b)
{
    if (!b){
        x=1; y=0; return a;
    }
    ll ans=Ex_Gcd(b, a%b, y, x);
    y-=a/b*x;
    return ans;
}

//数组长度len 模数c 余数r 所有c的lcm nc 最后答案nr
//方程有解返回1 不然返回0
bool Mod_Equation(ll len,ll* c,ll* r,ll&nr)
{
    ll x,y,nc=c[1]; 
    nr=r[1];
    for (int i=2;i<=len;i++)
    {
        ll dr=r[i]-nr;
        ll p=Ex_Gcd(nc, c[i], x, y);
        if (dr%p) return 0;
        ll t=c[i]/p;
        x=(dr/p*x %t +t) %t;
        nr+=nc*x;
        nc=nc*c[i]/p;
    }
    nr=(nr% nc+nc)%nc;
    return 1;
}

ll rev(int a,int Mo)
{
    ll x,y;
    Ex_Gcd(a,Mo,x,y);
    return (x%Mo + Mo)%Mo;
}
ll pw(ll a,int b,int Mo)
{
    if(!b) return 1;
    ll ans=pw(a,b/2,Mo);
    ans=ans*ans % Mo;
    if(b&1) ans=ans*a%Mo;
    return ans;
}
ll fac(int n,int p,int Mo,int& exp)// 去除素因子p的阶乘N
{
    if (!n) return 1;
    ll ans=1,a=1;
    for (int i=1;i<=Mo;i++) if(i%p) a=a * i % Mo;//这步根据情况考虑是否需要预处理
    while(n)
    {
        exp+=n/p;
        for (int i=1;i<=n%Mo;i++) if(i%p) ans=ans * i % Mo;//这步根据情况考虑是否需要预处理
        ans = ans * pw (a , n/Mo ,Mo) % Mo;         
        n/=p;
    }
    return ans;
}

ll calc(int n,int r,int Mo,int p,int c) // C(n,r)%Mo Mo=p^c p是素数
{
    int e1=0,e2=0,e3=0;
    ll f1=fac(n,p,Mo,e1);
    ll f2=fac(n-r,p,Mo,e2);
    ll f3=fac(r,p,Mo,e3);   
    if (e1-e2-e3>=c) return 0;
    return f1 * rev(f2,Mo) % Mo * rev(f3,Mo) % Mo * pw(p,e1-e2-e3,Mo) % Mo;
}

ll Lucas(int n,int r,int Mo)//Lucas定理 计算C(n,r)%Mo Mo 是素数并且要求Mo较小
{
    int x[50]={0},y[50]={0};
    
    while(n) x[++x[0]]=n % Mo , n/=Mo;
    while(r) y[++y[0]]=r % Mo , r/=Mo;
    ll ans=1;
    for (int i=1;ans && i<=y[0];i++)
        ans=ans * C(x[i],y[i]) %Mo;//C的计算需要预处理这里的C与下面的C无关！！
    return ans;
}
void Divide(int Mo)//分解质因数
{
    len=0;
    int lim=sqrt(Mo);
    for (int i=2;i<=lim;i++)
    if (Mo%i==0){
        p[++len]=i;
        m[len]=1;
        while(Mo % i ==0){
            Mo/=i;
            c[len]++;
            m[len]*=i;
        }
    }
    if (Mo==1) return;
    c[++len]=1;
    p[len]=m[len]=Mo;
}
ll C(int n,int r,int Mo)//计算C(n,r) % Mo 循环节法
{
    // Divide(Mo);//如果Mo都不变只需要在外部调用一次Divide
    ll ans;
    for (int i=1;i<=len;i++) 
        res[i]=calc(n,r,m[i],p[i],c[i]);
    Mod_Equation(len,m,res,ans);        
    return ans;

}
