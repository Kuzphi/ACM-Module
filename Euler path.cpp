#include <stdio.h>
#include <string.h>
#include <iostream>
#define Maxm 10010
#define Maxn 10010
using namespace std;
#define F(i,a,b) for (int i=a;i<=b;i++)
int                 e0,n,m,d[Maxn],top[Maxn],ans[Maxm];
struct Edge{
    int v,nt,vis,id;
}e[Maxm];
void add(int u,int v,int id)
{
    e[++e0].v=v;e[e0].nt=top[u]; top[u]=e0; e[e0].id=id; e[e0].vis=0;
    e[++e0].v=u;e[e0].nt=top[v]; top[v]=e0; e[e0].id=id; e[e0].vis=0;
}
void dfs(int u)
{
    for (int p=top[u];p;p=e[p].nt)
        if (!e[p].vis) {
            e[p].vis=1;
            if (p&1) e[p+1].vis=1; else e[p-1].vis=1;
            dfs(e[p].v);
            ans[++ans[0]]=e[p].id;
        }
}