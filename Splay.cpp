#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
const int Maxn=3000000,INF=1<<29;
struct SplayTree{
    int tl,Node_cnt;
    struct SplayNode{
        SplayNode *f,*s[2];
        int id;
        int  value,size,sum,max,min,msL,msR,msM,add,alt;
        bool isalt,rev;    
    }*root,*null,buff[Maxn];
    vector<SplayNode*> tmp;
    void Check(SplayNode *p){//检查并下放标记    
        if (p==null) return;
        if (p->rev){
            swap(p->msL,p->msR);
            swap(p->s[0],p->s[1]);
            p->s[0]->rev^=1;
            p->s[1]->rev^=1;   
            p->rev=0;
        }
        if (p->isalt){
            p->sum=p->alt * p->size;            
            p->msL=p->msR=p->msM=p->sum;
            p->value=p->min=p->max=p->alt;

            p->s[0]->isalt=p->s[1]->isalt=1;
            p->s[0]->alt=p->s[1]->alt=p->alt;

            p->s[0]->add=p->s[1]->add=0;
            p->isalt=0;
        }
        if (p->add){
            p->value += p->add;
            p->min += p->add;
            p->max += p->add;
            p->msL += p->add;
            p->msR += p->add;
            p->msM += p->add;
            p->sum += p->add * p->size;        
            p->s[0]->add+=p->add;
            p->s[1]->add+=p->add;
            p->add=0;
        }        
    }
    void Maintain(SplayNode *p){ //维护信息
        Check(p->s[0]);
        Check(p->s[1]);
        SplayNode *ls=p->s[0] , *rs=p->s[1];

        p->size = ls->size + rs->size +1;
        p->sum  = p->value + ls->sum + rs->sum;
        p->min  = min(p->value , min( ls->min , rs->min ));
        p->max  = min(p->value , min( ls->max , rs->max ));

        p->msL  = max(ls->msL  , max( ls->sum + p->value , ls->sum + p->value + rs->msL ));
        p->msR  = max(rs->msR  , max( rs->sum + p->value , rs->sum + p->value + ls->msR ));

        p->msM  = max(p->value , max( ls->msM , rs->msM));
        p->msM  = max(p->msM   , ls->msR + p->value + rs->msL );
        p->msM  = max(p->msM   , max( ls->msR + p->value , rs->msL+p->value ));
    }
    SplayNode* NewNode(SplayNode* f,int lr,int value){//f的lr儿子上新建一个值是value的节点
        SplayNode *p= buff+ (++tl);
        p->id=tl;
        f->s[lr] = p;
        p->f=f;
        p->value = value;
        p->s[0]  = p->s[1] = null;
        p->isalt = p->rev  = p->add = 0;
        Maintain(p);
        return p;
    }
    int LR(SplayNode *p){return p->f->s[1]==p;}// 判断是左子树还是右子树
    void Zxg(SplayNode *p){//旋转
        int k=LR(p);
        SplayNode *w=p->f, *ds=p->s[1-k];        

        Check(p);               // mark的维护
        Check(w);
        
        p->f=w->f;
        w->f->s[LR(w)]=p;
        w->s[k]=ds;
        ds->f=w;
        p->s[1-k]=w;
        w->f=p;
        
        Maintain(w);
        Maintain(p);
    }    
    void Splay(SplayNode *p,SplayNode *lim){// 伸展操作
        while (p->f!=lim){
            if (p->f->f!=lim)
                if (LR(p)==LR(p->f)) Zxg(p->f); else Zxg(p);
            Zxg(p);
        }
        if(lim == null) root=p;
    }
    SplayNode *Find(int k,SplayNode *lim){//查找序列中第x个数并旋转至lim下返回其指针    
        SplayNode *p = root;
        while(Check(p) , k != p->s[0]->size +1 ){
            if (k> p->s[0]->size+1){
                k-= p->s[0]->size+1 ;
                p=p->s[1];
            }
            else p=p->s[0];        
        }
        Splay(p,lim);
        return p;
    }
    SplayNode *Insert(int k,int v){//在k位置上插入元素v返回该元素的指针            
        Node_cnt++;
        SplayNode *x = Find(k+1,Find(k,null)), *p=NewNode(x,0,v);
        Maintain(x);Maintain(x->f);
        Splay(p,null);
        return p;
    }
    SplayNode *Insert(int k,int* v,int tot){//在k位置连续插入tot个元素返回最后元素的指针            
        Node_cnt+=tot;
        SplayNode *x = Find(k+1,Find(k,null)), *p=NewNode(x,0,v[1]);
        for (int i=2;i<=tot;i++) p=NewNode(p,1,v[i]);
        for (x = p; x!=null; x=x->f) Maintain(x);
        Splay(p,null);
        return p;
    }
    SplayNode* Lim_interval(int l,int r){//将l到r之间的数限制在一个子树中并返回这子树的根
        SplayNode *x=Find(l-1,null), *y=Find(r+1,x);
        return Check(y->s[0]),y->s[0];
    }
    void Delete(int l,int r=-INF){// 删除第X个数
        if (r==-INF) r=l;
        Node_cnt-=r-l+1;
        SplayNode *p=Lim_interval(l+1,r+1);
        p->f->s[0]=null;
        Maintain(p->f);
        Maintain(root);
    }
    int size(){return Node_cnt;} //返回节点个数
    int Min(int l,int r) {return Lim_interval(l+1,r+1)->min;}   // 返回区间lr的最小值
    int Max(int l,int r) {return Lim_interval(l+1,r+1)->max;}   //返回区间lr的最大值
    int Sum(int l,int r) {return Lim_interval(l+1,r+1)->sum;}   //返回区间lr的和
    int msM(int l,int r) {return Lim_interval(l+1,r+1)->msM;}   //区间最大连续子序列的和
    void Add(int l,int r,int d) {Lim_interval(l+1,r+1)->add+=d;}//区间lr之间的数值增加D
    void Reverse(int l,int r)   {Lim_interval(l+1,r+1)->rev^=1;}//翻转区间lr之间的所有数
    void Revolve(int l,int r,int d){//将区间lr中的后t个数放在前面
        d%=r-l+1;
        Reverse(l,r);Reverse(l,l+d-1);Reverse(l+d,r);
    }
    void Alter(int l,int r,int v){//修改第l到r数的值 改为v
        SplayNode *p=Lim_interval(l+1,r+1);
        p->isalt=1;
        p->alt=v;
        p->add=0;
    }
    void ToLim(SplayNode *p,SplayNode* lim){
    //外部调用的Splay函数,先check所有父节点再调用Splay,若无标记直接用上面的Spaly
        tmp.clear();
        for (SplayNode* x=p;x!=lim; x=x->f) tmp.push_back(x);
        for (int i=tmp.size()-1;i>=0;i--) Check(tmp[i]);        
        Splay(p,lim);
    }
    void Initialization(){//初始化,注意INF太大会有INF+INF爆int的情况
        tl=0;        
        null= buff;
        NewNode(root=NewNode(null,0,0),1,0);
        memset(null,0,sizeof(SplayNode));
        null->min=INF;
        null->max=null->msL=null->msR=null->msM=-INF;
    }
    void print(SplayNode *p){//中序遍历
        if (p==null) return;
        Check(p);
        print(p->s[0]);
        printf("%d ",p->value);
        print(p->s[1]);
    }
    SplayTree(){Initialization();}
}Splay;
 /* 
 维护12个操作
 1  插入
 2  删除
 3  序列的size
 4  查找第k个数       
 5  修改第k个数
 6  查询第某数的位置
 7  区间和
 8  区间翻转 
 9  区间流转
 10 区间最大值
 11 区间最小值 
 12 区间最大子序列的和
 */
