#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#define Maxn 30010
#define Maxm 150010
const int INF=2139062143;

using namespace std;
int n,m,e0;
struct edge{
    int v,c,nt;
}   e[Maxm];

int top[Maxn];
struct Node
{
    Node* l, *r,*f,*son;
    int dr,mk,key,id;//,use;
    void ins_Node(Node*& x){
        if (x==NULL) x=this;
        else {
            r=x->r;
            l=x;
            x->r->l=this;            
            x->r=this;
        }
    }
    Node (int x=0,int y=0):key(x),id(y),mk(0),dr(0),son(NULL),f(NULL),l(this),r(this){}//,use(0){}
    Node* BI_Tree_Merge(Node*p){
        p->ins_Node(son);
        p->f=this;
        dr++;
        p->mk=0;
        return this;
    }
    void List_delelte(){
        this->l->r=this->r;
        this->r->l=this->l;
        this->l=this->r=this;
    }
}*to[Maxn];
struct FIB_heap
{
    Node*Min;
    int n;
    FIB_heap():Min(NULL),n(0){}
    int get_Min(){ if (Min) return Min->key;  return -INF; }
    int get_id() { if (Min) return Min->id;   return -INF; }
    int size(){ return n; }
    Node* ins(int x,int y){
        n++;
        Node* p=new Node(x,y);
        p->ins_Node(Min);
        return p;
    }
    void Merge(FIB_heap& x)
    {
        n+=x.n;
        if (x.Min==NULL) return;
        if (Min==NULL) {
            Min=x.Min; 
            return;
        }
        Min->r->l=x.Min;
        x.Min->r->l= Min;
        swap(Min->r,x.Min->r);
        if (x.Min->key<Min->key) Min=x.Min;
    }
    void Consolidate()
    {
        int Max_dr=0;
        Node* D[20]={NULL};
        while(Min)
        {
            Node* p=Min;
            if (p->r==p) Min=NULL;
            else Min=p->r;
            p->List_delelte();
            while(D[p->dr]!=NULL)
            {
                int dr=p->dr;
                if (p->key < D[dr]->key) swap(p,D[dr]);
                p=D[dr]->BI_Tree_Merge(p);
                D[dr]=NULL;
            }
            D[p->dr]=p;
            Max_dr=max(Max_dr,p->dr);
        }
        for (int i=0;i<=Max_dr;i++) 
        if (D[i]){
            D[i]->ins_Node(Min);
            if (D[i]->key < Min->key) Min=D[i];
        }
    }
    void pop(){
        if (!Min) return;
        n--;        
        while(Min->son){
            Node* p=Min->son;
            if (p->r==p) Min->son=NULL;
            else Min->son=p->r;
            p->List_delelte();
            p->f=NULL;
            p->ins_Node(Min);
        }
        if (Min->r==Min) Min=NULL;
        else {
            Node* tmp=Min;
            Min=Min->r;
            tmp->List_delelte();
            Consolidate();
        }
    }
    void Cut(Node* x,Node* f)
    {
        if (x==x->r) f->son=NULL;
        else f->son=x->r;
        f->dr--;
        x->List_delelte();
        x->f=NULL;
        x->mk=0;
        x->ins_Node(Min);
    }
    void Casecading_Cut(Node* x)
    {   
        Node*p=x->f;
        if (p==NULL) return;
        if (p->mk){
            Cut(x,p);
            Casecading_Cut(p);
        }
        else p->mk=1;
    } 
    void Dec(Node*p ,int key){
        p->key=key;
        Node *f=p->f;
        if (p->f && key < p->f->key ){
            Cut(p,f);
            Casecading_Cut(f);            
        }
        if (key < Min->key) Min=p;
    }
    void Del(Node *p){
        Dec(p,-INF);
        pop();
    }
};
/*
void show_tree(FIB_heap Heap)
{
    if (Heap.Min)
        {
            Node* p=Heap.Min;
            do{
                cout<<p->id<<" "; p=p->r;
            }while(p!=Heap.Min); cout<<endl;
            for (int i=1;i<=n;i++) if (!to[i]->use) cout<<i<<"  "<<Get_id(to[i]->f)<<"  "<<Get_id(to[i]->son)
                                                           <<"  "<<Get_id(to[i]->r)<<"  "<<to[i]->dr<<endl;
            cout<<"Min : "<<Get_id(Heap.Min)<<endl;            
        }
        else puts("NULL");
}
*/
int dijkstra()
{
    FIB_heap Heap;
    for (int i=1;i<=n;i++) to[i]=Heap.ins(INF,i);
    Heap.Dec(to[1],0);
    int cnt=0;
    while(Heap.size())
    {
        int u=Heap.get_id();
        // to[u]->use=1;
        Heap.pop();
        int p=top[u];
        while(p)
        {
            int v=e[p].v;
            int c=e[p].c;
            p=e[p].nt;
            if (to[v]->key > to[u]->key+c)
                Heap.Dec(to[v],to[u]->key+c);             
        }
    }
    return to[n]->key;
}

void add(int u,int v,int c)
{
    e[++e0].v=v; e[e0].c=c; e[e0].nt=top[u]; top[u]=e0;
}

int main()
{
    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);
    while(~scanf("%d%d",&n,&m))
    {
        e0=0;
        memset(e,0,sizeof(e));
        memset(top,0,sizeof(top));
        for (int i=1;i<=m;i++) {
            int u,v,c;
            scanf("%d%d%d",&u,&v,&c);
            add(u,v,c);
        }
        int ans=dijkstra();
        printf("%d\n",ans);
    }
}