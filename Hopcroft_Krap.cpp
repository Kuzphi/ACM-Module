#include <vector>
#include <queue>
#include <stdio.h>
#include <iostream>
using namespace std;
const int                   Maxn=200010,Mo=1e9 + 7,oo=INT_MAX>>2;
struct Hopcroft_Krap{// sqrt(N) * M
    int nx,ny,dis;
    bool vis[Maxn];
    vector<int> e[Maxn];
    int cx[Maxn], cy[Maxn]; 
    int dx[Maxn], dy[Maxn];    

    bool BFS(){
        dis=oo;
        memset(dx, -1, sizeof(dx));
        memset(dy, -1, sizeof(dy));
        queue<int> Q;
        for(int i = 1; i <= nx; i++)
            if(cx[i] == -1){
                Q.push(i);
                dx[i] = 0;
            }
        while(Q.size()){
            int u = Q.front(); Q.pop();            
            if(dx[u] > dis) break;
            for (int i = 0 ; i < e[u].size(); i++){ 
                int v = e[u][i]; 
                if (dy[v] == -1){
                    dy[v] = dx[u] + 1; 
                    if(cy[v] == -1) dis = dy[v]; 
                    else{
                        dx[cy[v]]=dy[v]+1;
                        Q.push(cy[v]);
                    }
                }
            }
        }
        return dis != oo;
    }
    int Find(int u){
        for (int i = 0; i < e[u].size(); i++){
            int v = e[u][i];
            if (!vis[v] && dy[v] == dx[u] + 1){
                vis[v] = 1;
                if(cy[v] != -1 && dy[v] == dis)continue;
                if(cy[v] == -1 || Find(cy[v])){
                    cy[v] = u;
                    cx[u] = v;
                    return 1;
                }
            }
        }
        return 0;
    }
    int Match(){
        int ans = 0;
        memset(cx, -1, sizeof(cx));
        memset(cy, -1, sizeof(cy));
        while(BFS()){
            memset(vis, 0, sizeof(vis));
            for(int i = 1; i <= nx; i++)
                if(cx[i] == -1)
                    ans += Find(i);
        }
        return ans;
    }
    void build(int x, int y){//edge from x to y
        e[x].push_back(y);
    }
    void Init(int _nx,int _ny){//_nx,_ny is the size of set X and Y
        nx = _nx, ny = _ny;
        for (int i = 1; i <= nx; i++)
            e[i].clear();
    }
}HK;