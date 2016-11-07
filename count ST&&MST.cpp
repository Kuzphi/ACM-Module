#include <map>
#include <stack>
#include <queue>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#define inf 0x3f3f3f3f
#define LL long long 
#define clr(a,b) memset(a,b,sizeof(a))
using namespace std;
const int Maxn = 310 , Maxm = 1010 ;//Mo = 31011;

LL mat[Maxn][Maxn];
int S[Maxm*2] , G[Maxn][Maxn];
struct Edge{
    int u,v,c;
}e[Maxm];

LL Det(int n)//?????:Matrix-Tree??  
{  
    LL ret=1;  
    for(int i=1; i<=n; i++){  
        for(int j=i+1; j<=n; j++)          
            while(mat[j][i])  
            {  
                LL t=mat[i][i]/mat[j][i];                
                for(int k=i; k<=n; k++) mat[i][k]=( mat[i][k]-mat[j][k]*t ) ;//% Mo;  
                for(int k=i; k<=n; k++) swap(mat[i][k],mat[j][k]);  
                ret=-ret;  
            }  
        if(!mat[i][i]) return 0;  
        ret=ret * mat[i][i] ;//% Mo;  
    }  
    return ret;
}  
int N,M,f[Maxn];
int GF(int u){
    if (f[u] == u) return u;
    return f[u] = GF(f[u]);
}
int cmp(Edge a , Edge b){
    return a.c < b.c;
}
int cmpx(int a ,int b){
    if (GF(a)!=GF(b)) return GF(a)<GF(b);
    return a<b;
}
int main(){
    cin >> N >> M;
    for (int i=1;i<=M;i++)
    {
        int u,v,c;
        cin >> u >> v >> c;
        e[i].u = u , e[i].v = v; e[i].c = c;
        mat[u][u]++; mat[v][v]++;
        mat[u][v]--; mat[v][u]--;
    }
    // LL cnt = 1;
    LL all = Det(N-1) , cnt = 1;
    sort(e+1,e+M+1,cmp);   
    for (int i=1;i<=N;i++) f[i] = i;    
    for (int L=1,R=1;L<=M;L=R){        
        int vN = 0;
        S[0] = 0;
        for(;R<=M && e[L].c==e[R].c;R++);
        for (int i=L;i<R;i++){
            int fu = GF(e[i].u) , fv = GF(e[i].v);
            if (fu == fv) continue;
            G[fu][fv]--; G[fv][fu]--;
            G[fu][fu]++; G[fv][fv]++;
            S[++vN] = fu; 
            S[++vN] = fv;
        }        
        for (int i=L;i<R;i++) {
            int fu = GF(e[i].u) , fv = GF(e[i].v);
            f[fu]= fv;
        }
        sort(S+1,S+vN+1,cmpx);
        vN = unique(S+1,S+vN+1) - S - 1;
        for (int vL=1,vR=1;vR<=vN ;vL=vR){            
            int id = GF(S[vL]);            
            for(;vR<=vN && id == GF(S[vR]);vR++);            
            int Len = vR - vL;    
            for (int i=vL;i<vR;i++){
                for (int j=vL;j<vR;j++){
                    int u =i - vL + 1 , v = j - vL + 1;
                    mat[u][v] = G[S[i]][S[j]];
                }
            }                        
            cnt = cnt * Det(Len-1) ;
            G[id][id] = 0;
        }
    }
    int sum = 0;
    for (int i=1;i<=N;i++) if (GF(i)==i) sum++;
    if (sum>1) cnt = 0;
    LL t = __gcd(cnt,all);
    printf("%lld/%lld\n",cnt/t,all/t);  
}

