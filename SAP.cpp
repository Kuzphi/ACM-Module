#include <iostream>
#include <stdio.h>
#include <sstream>
#define CLR(x) memset(x,0,sizeof(x))
using namespace std;
const int                   Maxn=32000,Maxm=2500000,Mo=1000000007,oo=INT_MAX>>2;
struct Max_Flow{
    struct edge {
        int v,nt;double c;
    }e[Maxm];   
    int e0,s,t,cnt,top[Maxn],d[Maxn],vd[Maxn];
    void build(int u,int v,int c){
        e[++e0].v=v; e[e0].c=c; e[e0].nt=top[u]; top[u]=e0;
        e[++e0].v=u; e[e0].c=0; e[e0].nt=top[v]; top[v]=e0;
    }
    void Reset(){
        CLR(d); CLR(vd);
    }
    void Init(){
        Reset(); e0 = 1; CLR(top);//2-index
    }
    int Dfs(int u,int flow){
        if (u==t) return flow;
        int ans=0;
        for (int p = top[u];p;p=e[p].nt){
            int v=e[p].v , c=e[p].c;
            if (d[u]>d[v] && c>eps){
                int f=Dfs(v, min(flow-ans,c));
                ans+=f , e[p].c-=f , e[p^1].c+=f;
                if ( ans == flow ) return ans;
            }
        }
        if (!--vd[d[u]]) d[s]=cnt+2;
        vd[++d[u]]++;
        return ans;
    }
    int Solve(int _s,int _t,int _cnt){//_s起点标号 _t终点标号 _cnt图中点个数 
        int ans = 0;
        s=_s; t=_t; cnt=vd[0]=_cnt;
        while (d[s]<=cnt) ans+=Dfs(s, oo);
        return ans;
    }
}SAP;