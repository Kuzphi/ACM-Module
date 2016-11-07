#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Maxn 50010
using namespace std;

int Node_cnt;//节点总数量
int a[Maxn],cnt[Maxn],n;

struct Node{
    int l,r;
    //int f;//父亲节点 delete的时候有用
    int key,dis;
    Node (int x=0):key(x),dis(0),l(0),r(0){}//,f(0){}
} t[Maxn];

struct M_heap{
    int root,num;
    M_heap(){root=num=0;}
    inline int size(){return num;}
    inline void clear(){num=root=0;}
    inline int top(){return t[root].key;}
    inline int Mer(int A,int B){
        if (!A) return B;
        if (!B) return A;
        if (t[B].key > t[A].key) swap(A,B);        
        t[A].r=Mer(t[A].r,B);        
       // t[t[A].r].f=A; // 无delete可以删去
        if ( t[t[A].r].dis > t[t[A].l].dis) swap(t[A].l,t[A].r);
        t[A].dis = t[t[A].r].dis+1;
       //t[A].f=0;
        return A;
    }
    inline void Merge(M_heap& x){
        num+=x.num;
        root=Mer(root,x.root);
        x.clear();
    }
    inline void pop(){
        root=Mer(t[root].l,t[root].r);
        num--;
    }
    inline int ins(int& key){//插入一个数并返回指针
    
        num++;
        t[++Node_cnt]=Node(key);
        root=Mer(root,Node_cnt);        
        return Node_cnt;
    }    
    /*void Delete(int x)//delete 函数没有测过
    {
        int q=t[x].f;
        int p=Mer(t[x].l,t[x].r); 
        t[p].f=q;
        if (q) if (t[q].l==x) t[q].l=p; else t[q].r=p;
        while(q){
            if (t[t[q].l].dis < t[t[q].r].dis) swap(t[q].l,t[q].r);
            if (t[t[q].r].dis+1==t[q].dis) return;
            t[q].dis=t[t[q].r].dis+1;
            p=q;
            q=t[q].f;
        }
        num--;
    }*/
};
//注意本模版每次插入都是新开辟空间(无垃圾回收)t数组要开到N+M
void init(){
    Node_cnt=0;
    t[0].dis=-1;
}