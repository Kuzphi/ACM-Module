struct Pell{
    ll p,q;
    Pell();
    Pell(ll pp,ll qq){
        p=pp; q=qq;
    }
};
const int                   Maxn=100,Maxm=20000,Mo=7340033,oo=INT_MAX>>1;
typedef pair<int, int>      F;
typedef int Arr[Maxn];
int                         n;
Pell slove(int D)
{
    int pw=1;
    ll a1=(int)sqrt(D*1.0);
    Pell T(a1,D-a1*a1);
    ll a=(a1+T.p)/T.q;
    Pell W1(a1,1),W2(a1*a+1,a);
    while(T.q!=1||(pw%2==1))//如果是-1 那么pw%2==0
    {
        pw++;
        ll np=a*T.q-T.p;
        ll nq=(D-np*np)/T.q;
        T=Pell(np,nq);
        a=(a1+T.p)/T.q;
        Pell W3(W1.p+W2.p*a,W1.q+W2.q*a);
        W1=W2; W2=W3;
    }
    return W1;
}
int main()
{
    //注意精度 很可能会爆long long
    ios::sync_with_stdio(0);
    while (cin>>n)
    {
        int ck=(int)sqrt(n*1.0);
        if (ck*ck!=n)
        {
            Pell ans=slove(n);
            //cout<<ans.p<<' '<<ans.q<<endl;
        }
        else cout<<"no solution\n";
    }
}