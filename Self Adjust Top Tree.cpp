#include <iostream>
#include <cmath>
#include <cstring>
#include <queue>
#include <vector>
#include <cstdio>
using namespace std;
const int Maxn = 100100 , oo = 1 << 30;
struct SATT{//self adjust top tree
	int e0 , value[Maxn];
	struct Edge{
		int u,v,c;
	}e[Maxn];
	struct Node{
		struct Flag{
			int mul , add;
			Flag():mul(1){}
			Flag(int mul,int add):mul(mul),add(add){}
			bool Exist(){return add || (mul - 1);}
			void clear(){mul = 1, add = 0;}
			void operator += (const Flag &x) {
				mul *= x.mul;
				add = add * x.mul + x.add;
			}  
		}cF , tF;//ChainFlag , TreeFlag
		struct Info{
			int max , min , sum , sz;
			Info(){}
			Info(int max , int min , int sum ,int sz):max(max), min(min) , sum(sum), sz(sz){}
			Info operator += (const Flag &x){
				if (!sz) return *this;
				max = max * x.mul + x.add;
				min = min * x.mul + x.add;
				sum = sum * x.mul + x.add * sz;
				return *this;
			}
			void operator += (const Info &x){
				max = std::max(max , x.max);
				min = std::min(min , x.min);
				sum += x.sum;
				sz += x.sz;
			}
		}cI , tI , aI;//ChainInfo , TreeInfo , allInfo
		Node *s[4] , *f;
		int val;
		bool inr ,rev , use;
		void Init_Val(){
			aI = tI = cI = Info(-oo,oo,0,0);
			if (!inr) aI = cI = Info(val,val,val,1);
		}
		void Node_Init(int ty = 0,int v = 0){
			inr = ty;
			val = v;
			Init_Val();
			cF.clear();
			tF.clear();
			rev = 0;
			use = 1;
		}
		int Nson(){
			for (int i=0;i<4;i++) if (this->f->s[i] == this) return i;return 0;
		}
	}*null , *root , node[Maxn * 2];
	int tot , top , trash[Maxn];

	void Add_Edge(int u,int v,int c = 0){
		e[++e0].u=u;  e[e0].v = v; e[e0].c = c;
	}

    void Add_Value(int x ,int v){
        value[x] = v;
    }

    void Build(int N,int rt){
    	for (int i=1;i<=N;i++) NewNode(0,value[i],++tot);
    	for (int i=1;i<N;i++){
    		Make_Root(e[i].u);
    		Link(e[i].u, e[i].v);
    	}
    	Make_Root(rt);
    }
	Node* NewNode (int ty = 0 , int v = 0 , int id = -1){
		if (id < 0){
			if (top) id = trash[top--];
			else id = ++tot;
		}
		Node *p  = node + id;
		p->Node_Init(ty,v);
		p->f = null;
		fill(p->s , p->s + 4 , null);
		return p;
	}
	inline void Check(Node *p){// check flag
		if (p == null) return;
		if (p->rev){
			swap(p->s[0],p->s[1]);
			p->s[0]->rev^=1;
			p->s[1]->rev^=1;
			p->rev=0;
		}
        if (p->tF.Exist()){
			p->aI = p->tI += p->tF;
			p->aI += p->cI;
			for (int i=0;i<4;i++) 
				if (p->s[i]!=null) {
					p->s[i]->tF += p->tF;
					if (i >= 2) p->s[i]->cF += p->tF;
				}
			p->tF.clear();
		}
		if (p->cF.Exist()){
			p->aI = p->cI += p->cF;
			p->aI += p->tI;
			p->val = p->val * p->cF.mul + p->cF.add;
			for (int i=0;i<2;i++)
					p->s[i]->cF += p->cF;
			p->cF.clear();
		}
	}
	void Maintain(Node *p){//maintain information
		if (p==null) return;
		for (int i=0;i<4;i++) Check(p->s[i]);
		p->Init_Val();
		for (int i=0;i<2;i++) p->cI += p->s[i]->cI , p->tI += p->s[i]->tI; 
		for (int i=0;i<4;i++) p->aI += p->s[i]->aI;
		for (int i=2;i<4;i++) p->tI += p->s[i]->aI;
	}

	inline void SetSon(Node *u , Node *v , int r){
		if (u != null) u -> s[r] = v;
		if (v != null) v -> f = u;
	}
	void Zxg(Node* p, int ty = 0){
		int k = LR(p , ty);
		Node* w = p ->f;
		SetSon(w->f , p , w->Nson());
		SetSon(w , p->s[!k + ty] , k + ty);
		SetSon(p , w , !k + ty);
		Maintain(w);
	}
	int LR(Node * p , int ty = 0){
		return p -> f -> s[1  + ty] == p;
	}
	bool IsRt(Node *p , int ty = 0){//whether p is root
		if (ty) return !p->f->inr || !p->inr;
		else return p->f == null || (p->f->s[0]!=p&&p->f->s[1]!=p);
	}
	void Splay(Node* p,int ty = 0){
		while(!IsRt(p,ty)){
			if (!IsRt(p->f,ty)){
				if(LR(p , ty) == LR(p -> f ,ty)) Zxg(p -> f,ty); else Zxg(p,ty);
			}
			Zxg(p,ty);
		}
		Maintain(p);
	}
	void Add(Node* u , Node *v){//put v into u's virtual tree
		if (v == null) return;
		Check(u);
		Check(v);
		for (int i=2;i<4;i++) 
			if (u->s[i] == null) {
				SetSon(u , v , i);
				return;
			}
		Node* p = NewNode(1,0) , *x;
		for (x = u; x->s[2]->inr; Check(x->s[2]) , x = x->s[2]);
		SetSon(p , v , 3);
		SetSon(p , x->s[2] , 2);
		SetSon(x , p , 2);
		Splay(p , 2);
	}
	void Del(Node *p){//delete node p
		if (!p->f->inr) SetSon(p->f , null , LR(p , 2) + 2);
		else {
			SetSon(p->f->f , p->f->s[3 - LR(p,2)] , LR(p->f,2) + 2);
			trash[++top] = p->f - node;
			p->f->use = 0;
			Splay(p->f->f,2);
		}
		p->f = null;
	}
	void Clear_Mark(Node *p){
		if (p == null) return;
		Clear_Mark(p->f);
		Check(p);
	}
	void Splice(Node *p){
		Splay(p);
		Node * w = p->s[0];
		if (w == null) return;
		w->f = p;
		p->s[0] = null;
		Add(p , w);
		Maintain(p);
	}

	Node *Access(Node *p){
		Node*w = p, *r;
		Clear_Mark(p);
		Splice(w);
		for (; w->f!=null;w = r){
			for (r = w->f;r->inr; r=r->f);
			Splice(r);
			Del(w);
			SetSon(r , w , 0);	
			Maintain(r);		
		}
		Splay(p);
		return w;
	}

    void __Make_Root(Node *p){
		Access(p);
		p->rev ^= 1;
    }
    void Make_Root(int x){
        root = node + x;
        __Make_Root(root);
    }
	Node* Findf(Node *x){while(x->f!=null) x = x->f; return x;}
	Node* Cut(int x){//split x and x's father
    	Node* u = node + x;
    	Access(u);
    	Check(u->s[1]);    	
    	for (u = u->s[1];u!=null && u->s[0]!=null; u=u->s[0], Check(u));
    	if (u!=null) Access(u) , Del(node + x) , Maintain(u);
    	return u;
    }
    void Link(int x ,int y){//connect x and y
		Node *p = Cut(x) , *u = node + x, *v = node + y;		
		if (Findf(u) != Findf(v)) p = v;
		if (p==null) return;
		Access(p);
		Add(p,u); 
		Maintain(p);
	}
	Node::Info Chain(int u ,int v,int mul,int add){
		__Make_Root(node + u);
		Access(node + v);
		Splay(node + u);
		node[u].cF += Node::Flag(mul,add);
		Node::Info ans = node[u].cI;
		__Make_Root(root);
		return ans;
	}
	Node::Info Tree_Ask(int u){
		Access(node + u);
		int val = node[u].val;
		Node::Info ans = Node::Info(val,val,val,0);
		for (int i=2;i<4;i++) Check(node[u].s[i]) , ans += node[u].s[i]->aI;
		return ans;
	}

	void Tree_Alter(int u , int mul,int add){
		Access(node + u);
		node[u].val = node[u].val * mul + add;
		for (int i=2;i<4;i++) {
			node[u].s[i]->tF += Node::Flag(mul , add);
			node[u].s[i]->cF += Node::Flag(mul , add);
		}
		Maintain(node + u);
	}
    int Chain_Sum(int u,int v) {return Chain(u,v,1,0).sum;}
    int Chain_Min(int u,int v) {return Chain(u,v,1,0).min;}
    int Chain_Max(int u,int v) {return Chain(u,v,1,0).max;}
    void Chain_Alter(int u,int v,int mul,int add) {Chain(u,v,mul,add);}    

    int Tree_Min(int u) {return Tree_Ask(u).min;}
    int Tree_Max(int u) {return Tree_Ask(u).max;}
    int Tree_Sum(int u) {return Tree_Ask(u).sum;}
 
    void Initialization(){
        e0 = top = 0;
        null = node;
        NewNode(1 , 0 , 0);
        null->inr = 0;
    }
    SATT(){ Initialization(); }
    void Show(){
    	for (int i=1;i<=tot;i++){
    		if (!node[i].use) continue;
    		cout << i <<": ";
    		for (int j=0;j<4;j++)
    		cout << node[i].s[j] - node <<" ";
    		cout <<" f: "<< node[i].f-null
    		<<" rev: "<<node[i].rev<<" val : "<<node[i].val <<" "
    		<<" aSum: "<<node[i].aI.sum << " tSum: "<<node[i].tI.sum<<" cSum: "<<node[i].cI.sum<< endl;    		
    	}
    	puts("--------------------------");
    }
}SATT_Tree;
int N , M , v;
int main(){	
	scanf("%d%d",&N,&M);
	for (int i=1;i<N;i++){
		int u ,v;
		scanf("%d%d",&u,&v);
		SATT_Tree.Add_Edge(u,v);
	}
	for (int i=1;i<=N;i++){
		scanf("%d",&v);
		SATT_Tree.Add_Value(i , v);
	}
	int rt;
	scanf("%d",&rt);	
	SATT_Tree.Build(N,rt);	
	for (int i=1;i<=M;i++){
		int op, x, y , c;
		scanf("%d", &op);	
		if (!op){
			scanf("%d%d",&x,&c);			
			SATT_Tree.Tree_Alter(x,0,c);
		}
		if (op == 1){
			scanf("%d",&x);			
			SATT_Tree.Make_Root(x);
		}
		if (op == 2){
			scanf("%d%d%d",&x,&y,&c);
			SATT_Tree.Chain_Alter(x,y,0,c);
		}
		if (op == 3){
			scanf("%d",&x);
			printf("%d\n", SATT_Tree.Tree_Min(x));
		}
		if (op == 4){
			scanf("%d",&x);
			printf("%d\n",SATT_Tree.Tree_Max(x));
		}
		if (op == 5){						
			scanf("%d%d",&x,&c);
			SATT_Tree.Tree_Alter(x,1,c);
		}
		if (op == 6){
			scanf("%d%d%d",&x,&y,&c);
			SATT_Tree.Chain_Alter(x,y,1,c);
		}
		if (op == 7){
			scanf("%d%d",&x,&y);
			printf("%d\n",SATT_Tree.Chain_Min(x,y));
		}
		if (op == 8){
			scanf("%d%d",&x,&y);
			printf("%d\n",SATT_Tree.Chain_Max(x,y));
		}
		if (op == 9){
			scanf("%d%d",&x,&y);
			SATT_Tree.Link(x , y);
		}
		if (op == 10){
			scanf("%d%d",&x,&y);
			printf("%d\n",SATT_Tree.Chain_Sum(x,y));
		}
		if (op == 11){
			scanf("%d",&x);
			printf("%d\n",SATT_Tree.Tree_Sum(x));
		}
	}
}