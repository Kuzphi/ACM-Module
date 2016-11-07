#include<iostream>
using namespace std;
struct edge{
	int v,c,w,Ft;
};
const int 			Maxn=300,Maxm=100000,INF=(long long)(1<<31)-1;
edge				e[Maxm];
int 				i,j,n,m,s,t,e0,ans,
					d[Maxn],top[Maxn];
void build(int u,int v,int c,int w){
	e[++e0].v=v; e[e0].c=c; e[e0].w=w;  e[e0].Ft=top[u]; top[u]=e0;
 	e[++e0].v=u; e[e0].c=0; e[e0].w=-w; e[e0].Ft=top[v]; top[v]=e0;
}
int Cost_Flow(){
    bool				vis[Maxn];
    int                 Flow[Maxn],Ft[Maxn],st[Maxn],d[Maxn],vd[Maxn];
	int hd,tl,v;
	memset(vis,0,sizeof(vis));
	for (int i=0;i<=Maxn;i++) d[i]=INF;
	st[1]=s;
	hd=d[s]=0; tl=1; vis[s]=1; Flow[s]=INF;
	while (hd!=tl){
		hd=hd %Maxn +1;
		int u=st[hd],p=top[u];
		while (p){
			v=e[p].v;
			if (e[p].c>0&&d[v]>d[u]+e[p].w){
				d[v]=d[u]+e[p].w;
				Flow[v]=min(Flow[u],e[p].c);
				vd[v]=p; Ft[v]=u;
				if (!vis[v]){
					vis[v]=1;
					tl=tl%Maxn+1;
					st[tl]=v;
				}
			}
			p=e[p].Ft;
		}
		vis[u]=0;
	}
	if (d[t]==INF) return 0;
	ans+=d[t]*Flow[t];
	v=t;
	do{
		int p=vd[v];
		e[p].c-=Flow[t];
		if (p&1) e[p+1].c+=Flow[t];
		else e[p-1].c+=Flow[t]; 
		v=Ft[v];
	}while (v!=s);
	return 1;
} 
int main()
{
	/*s是起点 t是终点*/
    while (Cost_Flow());/* 调用*/
    /* 调用后ans为最后答案*/
} 