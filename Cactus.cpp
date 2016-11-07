#include <math.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <limits.h>
#include <algorithm>
#define X first 
#define Y second 
#define MP make_pair
#define PB push_back
#define ll long long
#define feach(i, v)   for(int i = 0; i < v.size(); i++)
#define rep(i, a, b)  for(int i = a; i <= b; i++)
#define drep(i, a, b) for(int i = a; i >= b; i--)
using namespace std;
const int Maxn = 100010, Mo = 1e9 + 7, INF = INT_MAX >> 2;
int n, m, p;
struct Cactus{
    struct Edge{
        int v,id;
        ll c;
        Edge(){} 
        Edge(int v, ll c, int id):v(v),c(c),id(id){}
    };
    vector<Edge> e[Maxn];
    //ecnt: number of edge
    //d: distance to the circle
    //clr: point in circle
    //cid: the edge pointer to the next circle point
    //wclr: belong to which circle point
    int ecnt;
    ll d[Maxn], sum[Maxn];
    int stk[Maxn], idstk[Maxn];
    int clr[Maxn], cid[Maxn], wclr[Maxn], pos[Maxn];
    bool instk[Maxn], inclr[Maxn];

    int circle(int u, int pid){
        stk[++stk[0]] = u;
        instk[u] = 1;
        feach(i, e[u]){
            int v = e[u][i].v, id = e[u][i].id;
            if (id == pid) continue;
            idstk[stk[0]] = i;
            if (instk[v]){
                for(int i = stk[0]; clr[clr[0]] != v;i--){
                    clr[++clr[0]] = stk[i];                    
                    cid[clr[0]] = idstk[i];
                    inclr[stk[i]] = 1;
                }
                reverse(clr + 1, clr + clr[0] + 1);
                reverse(cid + 1, cid + clr[0] + 1);
                return 1;
            }
            if (circle(v, id)) return 1;
        }
        stk[0]--;
        return instk[u] = 0;
    }
    void dfs(int u, int f, ll dis, int which_clr){
        d[u] = dis;
        wclr[u] = which_clr;
        feach(i, e[u]){
            int v = e[u][i].v, c = e[u][i].c;
            if (v== f || inclr[v]) continue;
            dfs(v, u, dis + c, which_clr);
        }
    }
    void Insert(int u , int v ,int c){//1-index base
        ecnt++;
        e[u].PB(Edge(v, c, ecnt));
        e[v].PB(Edge(u, c, ecnt));
    }
    void Build(){
        circle(1,-1);
        clr[clr[0] + 1] = clr[1];
        rep(i,1,clr[0])            
            dfs(clr[i],0,0, clr[i]);        
        rep(i,1,clr[0]){
            pos[clr[i]] = i;
            sum[i + 1] = sum[i] + e[clr[i]][cid[i]].c;
        }
    }
    void Init(int n){
        ecnt = 0;
        stk[0] = clr[0] = 0;
        rep(i, 1, n) e[i].clear();
        memset(sum,0,sizeof(sum));
        memset(inclr,0,sizeof(inclr));
        memset(instk,0,sizeof(instk));
    }
    bool InCircle(int u){return inclr[u];}
    ll CircleLen(){return sum[clr[0] + 1];}
    ll CircleDis(int u ,int v){
        return sum[pos[v]] - sum[pos[u]];
        //or CircleLen - (sum[pos[v]] - sum[pos[u]])
    }
}Cactus;
