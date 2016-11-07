#include <iostream>
#include <string.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <vector>
#include <string>
#define PB(x) push_back((x))
#define X first
#define Y second
#define MP(x,y) make_pair((x),(y))
using namespace std;
const int                   Maxn=500000,Maxm=1000,Mo=7340033,oo=INT_MAX>>1;
typedef pair<int, int> F;
typedef int Arr[Maxn];
Arr                         use,t,s,vis,l,r;
int                         n,w,d,i;
long long                   ans;
vector<F>                   e[Maxn],a[Maxn];
int ask(int k,int l,int r,int x,int y)
{
    if (y<l||r<x) return 0;
    if (x<=l&&r<=y)
        return t[k];
    int mid=(l+r)/2;
    return ask(k*2,l,mid,x,y)+ask(k*2+1,mid+1,r,x,y);
}
void ins(int k,int l,int r,int x,int key)
{
    if (x<l||r<x) return;
    if (l==r)
    {
        t[k]+=key; return;
    }
    int mid=(l+r)/2;
    ins(k*2,l,mid,x,key);
    ins(k*2+1,mid+1,r,x,key);
    t[k]=t[k*2+1]+t[k*2];
}
void calc(vector<F>&a,vector<F>&b)//统计函数 根据实际需要写
{
    int p=-1;
    for (int i=(int)a.size()-1;i>=0;i--)
    {
        while(p+1<b.size()&&a[i].X+b[p+1].X<=w)
            ins(1,0,d,b[++p].Y,1);
        ans+=ask(1,0,d,0,d-a[i].Y);
    }
    for(int i=0;i<=p;i++) ins(1,0,d,b[i].Y,-1);
}
F find(int u,int f,int sum){//找到树的重心 sum表示这棵树的总结点数目
    s[u]=1;
    int lar=0;
    F ans=MP(oo,0);
    for (int i=0;i<e[u].size();i++){
        int v=e[u][i].Y;
        if (v==f||vis[v]) continue;
        F x=find(v,u,sum);
        s[u]+=s[v];
        lar=max(lar,s[v]);
        ans=min(ans,x);
    }
    lar=max(lar,sum-s[u]);
    ans=min(ans,MP(lar,u));
    return ans;
}
void dfs(int u,int f,int x,int y,vector<F> &a){
    a.PB(MP(x,y));
    s[u]=1;
    for (int i=0;i<e[u].size();i++){
        int v=e[u][i].Y,c=e[u][i].X;
        if (v==f||vis[v]) continue;
        dfs(v,u,x+c,y+1,a);
        s[u]+=s[v];
    }
}
void dfs(int u,int f,int sum){
    u=find(u,f,sum).Y;
    int m=0,left,x=-1;
    for (int i=0;i<e[u].size();i++){//深搜 得到各个子树的信息
        int v=e[u][i].Y,c=e[u][i].X;
        if (vis[v]) continue;
        a[++m].clear();
        dfs(v,u,c,1,a[m]);
        sort(a[m].begin(),a[m].end());
    }
    a[++m].clear();
    a[left=m].PB(MP(0,0));
    while(left>1){//合并各个子树的信息
        int  d = left/2;
        for (int i=1;i<=d;i++){
            calc(a[i],a[i+d]);
            vector<F> b(a[i].size()+a[i+d].size());
            merge(a[i].begin(),a[i].end(),a[i+d].begin(),a[i+d].end(),b.begin());
            a[i]=b;
        }
        if (left&1) a[++d] = a[left];
        left= d;
    }
    vis[u]=1;//vis 表示这个节点已经不再树内
    for (int i=0;i<e[u].size();i++){
        int v=e[u][i].Y;
        if (!vis[v]&&s[v]>1) dfs(v,u,s[v]);
    }
}
