const int                           Maxn=200010,Maxm=300005,Mo=1000000007,
sp[8][2]={{-1,0},{1,0},{0,1},{0,-1},{1,1},{-1,-1},{1,-1},{-1,1}};
const ll                            oo=(1LL<<62);
typedef pair<int ,int>                  F;

typedef int                             IArr[Maxn];
typedef ll                              LArr[Maxn];
typedef double                          DArr[Maxn];

struct SA {
    int fl,nt[27];
    int dep;
    
} sa[Maxn];

int total,tl;
void init()
{
    CLR(sa);
    fill(sa[0].nt,sa[0].nt+27,1);
    sa[0].dep=-1;
    total=tl=1;
}
void ins(char ch)
{
    int c=ch-'a';
    int p=tl;
    int np=++total;
    sa[np].dep=sa[p].dep+1;
    for (;!sa[p].nt[c];p=sa[p].fl) sa[p].nt[c]=np;
    tl=np;
    int q=sa[p].nt[c];
    if (sa[p].dep+1==sa[q].dep) sa[np].fl=q;
    else {
        int nq=++total;
        sa[nq]=sa[q];
        sa[nq].dep=sa[p].dep+1;
        sa[q].fl=sa[np].fl=nq;
        for (;sa[p].nt[c]==q;p=sa[p].fl)
            sa[p].nt[c]=nq;
    }
}
int                                     num=0;
int main()
{
    string s1,s2;
    init();
    cin>>s1>>s2;
    for (int i=0;i<s1.size();i++) ins(s1[i]);
    for (int i=0;i<s1.size();i++) ins(s1[i]);
    int p=1,ans=0,cnt=0;
    cout<<ans<<endl;
}