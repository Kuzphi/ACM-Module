#include <iostream>
#include <cmath>
#include <cstring>
#include <queue>
#include <string>
#include <vector>
#include <cstdio>
#define PB push_back
#define LS k*2 , l , mid
#define RS k*2 + 1 , mid + 1 , r
using namespace std;
const int Maxn = 50010 , oo = 1 << 30;
struct SLPF{
    struct Node{
        int dep , top , sz , f , val ,hs , id;
        void Set(int _dep ,int _f){
            f = _f; dep = _dep; top = hs = id = 0; sz = 1;
        }
    }w[Maxn], *id[Maxn];
    struct Flag{
        int mul , add;
        Flag(){mul = 1;}
        Flag(int mul,int add):mul(mul),add(add){}
        Flag operator +=(Flag& a){
            return *this = Flag(mul * a.mul , add * a.mul + a.add);
        }
        bool Exist(){return mul - 1 || add; }
    };
    struct Info{
        int sum , max ,min,sz;
        Info(){}
        Info(int val,int sz):sum(val),max(val),min(val),sz(sz){}
        Info(int sum,int max,int min,int sz):sum(sum),max(max),min(min),sz(sz){}
        Info operator +(Info& a){
            return Info(a.sum + sum , std::max(a.max,max),std::min(a.min,min),sz + a.sz);
        }
        Info operator +=(Flag& a){
            return *this = Info(sum * a.mul + sz * a.add ,max * a.mul + a.add,min*a.mul + a.add,sz);
        }
    };
    struct sNode{
        Flag fl; Info info;
    }t[Maxn * 4];
    int N , cnt;
    vector<int> e[Maxn];
    void Add_Edge(int u,int v){
        e[u].PB(v), e[v].PB(u);
    }
    void Add_Value(int pos,int val){
        w[pos].val = val;
    }
    void PreWork(int tot){
        N = tot;
        dfs1(1,1,1);
        dfs2(1,1);
        Build(1,1,N);
    }
    void dfs1(int u, int f, int d) {
        w[u].Set(d , f);
        for (int i = 0; i < e[u].size(); i++) {
            int v = e[u][i];
            if (v == f) continue;
            dfs1(v, u, d + 1);
            w[u].sz += w[v].sz;
            if (w[v].sz > w[w[u].hs].sz) w[u].hs = v;
        }
    }
    void dfs2(int u, int top) {
        w[u].top = top;
        w[u].id = ++ cnt;
        id[cnt] = w + u;
        if (w[u].hs) dfs2(w[u].hs, top);
        for (int i = 0; i < e[u].size(); i++) {
            int v = e[u][i];
            if (v == w[u].f || v == w[u].hs) continue;
            dfs2(v, v);
        }
    }
    void Build(int k,int l,int r){
        if (l == r){
            t[k].fl=Flag(1,0);
            t[k].info= Info(id[l]->val,r - l + 1);
            return;
        }
        int mid = (l + r) >> 1;
        Build(LS);
        Build(RS);
        t[k].fl=Flag(1,0);
        t[k].info = t[k*2].info + t[k*2 + 1].info;
    }
    void Check(int k,int l,int r){
        if(!t[k].fl.Exist()) return;
        t[k].info += t[k].fl;
        if (l!=r){
            t[k*2].fl   += t[k].fl;
            t[k*2+1].fl += t[k].fl;
        }
        t[k].fl = Flag(1,0);
    }
    void Query(int k,int l,int r,int x ,int y, Info& ans){
        Check(k,l,r);
        if (r < x || y < l) return;
        if (x<=l && r<=y){
            ans = ans + t[k].info;
            return;
        }
        int mid = (l + r) >>1;
        Query(LS,x,y,ans);
        Query(RS,x,y,ans);
    }
    void Modify(int k,int l,int r,int x ,int y, Flag& fl){
        Check(k,l,r);
        if (r < x || y < l) return;
        if (x<=l && r<=y){
            t[k].fl += fl;
            Check(k,l,r);
            return;
        }
        int mid = (l + r) >>1;
        Modify(LS,x,y,fl);
        Modify(RS,x,y,fl);
        t[k].info = t[k*2].info + t[k*2 + 1].info;
    }
    Info Ask(int u,int v,Flag c = Flag(1,0)){
        Info ans(0,-oo,oo,0);    
        for (int fu = w[u].top , fv = w[v].top; fu!=fv; u = w[fu].f,fu = w[u].top){
            if (w[fu].dep < w[fv].dep) swap(u,v) , swap(fu,fv);
            if (c.Exist()) Modify(1,1,N,w[fu].id,w[u].id,c);
            else Query(1,1,N,w[fu].id,w[u].id,ans);
        }
        if (w[u].id > w[v].id) swap(u,v);
        if (c.Exist()) Modify(1,1,N,w[u].id,w[v].id,c);
        else Query(1,1,N,w[u].id,w[v].id,ans);
        return ans;
    }
    void Initialization(){
        for (int i=1;i<=N;i++) e[i].clear();
        cnt = N = 0;
    }
}slpf;
int N,M,Q;
char st[20];
int main(){    
    while(cin >> N >> M >> Q){
        slpf.Initialization();
        for (int i=1;i<=N;i++){
            int val;
            scanf("%d",&val);
            slpf.Add_Value(i,val);
        }
        for (int i=1;i<N;i++){
            int u , v;
            scanf("%d%d",&u,&v);
            slpf.Add_Edge(u,v);
        }        
        slpf.PreWork(N);
        while(Q--){
            string op;
            cin >> op;
            int x,y,c;
            scanf("%d",&x);
            if (op == "I" || op =="D") scanf("%d%d",&y,&c);
            if (op == "I")
                slpf.Ask(x , y , SLPF::Flag(1,c));
            else if (op== "D")
                slpf.Ask(x , y , SLPF::Flag(1,-c));
            else
                printf("%d\n",slpf.Ask(x , x).sum);
        }
    }

}