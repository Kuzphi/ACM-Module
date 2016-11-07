#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <string.h>
using namespace std;
#define ll long long
const   int                 Maxn=100010, Mo = 51061 , INF=INT_MAX>>2;
struct Link_Cut_Tree{
    int e0 , top[Maxn] , value[Maxn];
    struct Edge{
        int v,nt,c;
    }e[Maxn * 2];
    struct TreeNode{
        TreeNode *f,*s[2];
        unsigned int  value,size,sum,max,min,add,alt,mul;
        bool isalt, rev, ck;
    }*null,root,node[Maxn];
    void NewNode (TreeNode *p ,int f,int v){
        memset(p,0,sizeof(TreeNode));
        p->ck = p-> mul  = 1;
        p->value = p->sum = v;
        p->f = node + f;
        p->s[0] = p->s[1] = null;
        Maintain(p);
    }
    int LR(TreeNode *p){ return p->f->s[1]==p; }

    void Add_Edge(int u,int v,int c = 0){
        e[++e0].v=v; e[e0].nt=top[u]; e[e0].c = c; top[u]=e0;
        e[++e0].v=u; e[e0].nt=top[v]; e[e0].c = c; top[v]=e0;
    }
    void Add_value(int x ,int v){
        value[x] = v;
    }
    void Build(int u , int f){
        NewNode(node + u , f , value[u]);
        for (int p=top[u]; p ; p= e[p].nt){
            int v=e[p].v , c = e[p].c;
            if (v == f) continue;
            Build(v,u);
        }
    }
    void Check(TreeNode *p){//???????
        if (p==null) return;
        if (p->rev){
            swap(p->s[0],p->s[1]);
            p->s[0]->rev^=1;
            p->s[1]->rev^=1;   
            p->rev=0;
        }
        // if (p->isalt){
        //     p->sum=p->alt * p->size;
        //     p->value=p->min=p->max=p->alt;

        //     p->s[0]->isalt=p->s[1]->isalt=1;
        //     p->s[0]->alt=p->s[1]->alt=p->alt;

        //     p->s[0]->add=p->s[1]->add=0;
        //     p->isalt=0;
        // }
        if (p->mul - 1){
            (p->value *= p->mul) %= Mo;
            (p->sum *= p->mul) %= Mo;          
            // (p->min *= p->mul) %= Mo;
            // (p->max *= p->mul) %= Mo;
            (p->s[0]->add *= p->mul) %= Mo;
            (p->s[1]->add *= p->mul) %= Mo;
            (p->s[0]->mul *= p->mul) %= Mo;
            (p->s[1]->mul *= p->mul) %= Mo;
            p->mul=1;
        }
        if (p->add){
            (p->value += p->add) %= Mo;
            // p->min += p->add;
            // p->max += p->add;
            (p->sum += p->add * p->size) %= Mo;
            (p->s[0]->add += p->add) %= Mo;
            (p->s[1]->add += p->add) %= Mo;
            p->add=0;
        }
    }
    void Maintain(TreeNode *p){     
        Check(p->s[0]);
        Check(p->s[1]);
        TreeNode *ls=p->s[0] , *rs=p->s[1];
        (p->size = ls->size + rs->size +1)%=Mo;
        (p->sum  = p->value + ls->sum + rs->sum)%=Mo;
        //p->min  = min(p->value , min( ls->min , rs->min ));
        //p->max  = max(p->value , max( ls->max , rs->max ));
    }
    void Zxg(TreeNode* p){
        int k=LR(p);
        TreeNode *w=p->f, *ds=p->s[1-k];

        Check(p);               // mark???
        Check(w);

        p->f=w->f;
        if(!w->ck) w->f->s[LR(w)] = p;
        w->s[k]=ds;
        ds->f=w;
        p->s[1-k]=w;
        w->f=p;

        p->ck = w->ck;
        w->ck = 0;
        Maintain(w);
        Maintain(p);
    }
    void dfs(TreeNode *p){
        if (!p->ck) dfs(p->f);
        Check(p);
    }
    void Splay(TreeNode *p){// ????
        dfs(p);
        while (!p->ck){
            if (!p->f->ck){
                if (LR(p)==LR(p->f)) Zxg(p->f); else Zxg(p);
            }
            Zxg(p);
        }
    }
    void Splice(TreeNode *p){       
        Splay(p);
        p->s[0]->f = p;
        p->s[0]->ck = 1;
        p->s[0] = null;
        Maintain(p);
    }
    TreeNode* Access(TreeNode *p){
        Splice(p);
        for(;p->f!=null;p=p->f){
            Splice(p->f);
            p->ck=0;
            p->f->s[0]=p;
            Maintain(p->f);
        }
        return p;
    }   
    TreeNode* Combine(int u,int v){
        Access(node + v);
        TreeNode * LCA = Access(node + u);
        Splay(node + v);
        NewNode(&root,0,0);         
        TreeNode *p = LCA->s[1];
        for (;p!=null && p->s[0]!=null;p=p->s[0]);
        if (p!=null){
            Splay(p);
            Splice(LCA->f);
        }
        root.s[0] = LCA;
        root.s[1] = (node + v)== LCA? null : node + v;      
        Maintain(&root);
        return &root;
    }
    void Initialization(){
        e0 = 0;
        memset(top,0,sizeof(top));
        null = node;
        memset(null,0,sizeof(TreeNode));
        null->min=INF;
        null->max=-INF;
    }
    
    void Modify(int x, int v){
        TreeNode *p = node + x;
        Splay(p);
        p->value = v;
        Maintain(p);
    }
    void Make_Root(int x){
        TreeNode *p = node + x;
        Access(p);
        Splay(p);
        p->rev ^= 1;
    }
    void Link(int x ,int y){
        TreeNode *u = node + x, *v = node + y;
        Make_Root(x);
        u->f = v;
    }
    void Cut(int x,int y){
        TreeNode *u = node + x , *v = node + y;
        Access(v); Splay(u);
        if (u->f == v) u->f = null;
        else {
            Access(u); Splay(v);
            v->f = null;
        }
    }
    int Get_Sum(int u,int v){ return Combine(u,v) -> sum; }
    void Add(int u,int v,int c){ 
        Combine(u,v)->add = c; Check(&root);
    }
    void Mul(int u,int v,int c){
        Combine(u,v)->mul = c; Check(&root);
    }
    Link_Cut_Tree(){ Initialization(); }
}LCT;
/*
    建树过程：
    Add_Edge 加入树边
    Add_value 加入点权
    Build(1,0) 建树
    然后再调用各种操作
    Modify(x,v) x的点权修改为v
    Make_Root(x) x置为根
    Link(x,y) x,y加边
    Cut(x,y) 删除x,y的边(一定要存在)
    Add(u,v,c) u,v链上每个数+c
    Mul(u,v,c) u,v链上每个数*c
    Get_sum(u,v) 求u,v链上的和
*/