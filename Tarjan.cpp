#include <stdio.h>
#include <iostream>
using namespace std;
const int 				Maxn=300000,Maxm=300000,INF=(long long)(1<<31)-1;
int 					n,m,u,v,i,ans,tot,e0,num,t,
						s[Maxn],dfn[Maxn],low[Maxn],val[Maxn],top[Maxn],col[Maxn],e[Maxm*3][3];
bool 					bo[Maxn],vis[Maxn],instack[Maxn];
void build(int u,int v)
{
	e[++e0][0]=u; e[e0][1]=top[v]; top[v]=e0;
}
void dfs(int u){
	dfn[u]=low[u]=t++;
	vis[u]=instack[u]=1;
	s[++s[0]]=u;
	int p=top[u];
	while (p){
		int v=e[p][0];
		if (!vis[v])
			dfs(v),low[u]=min(low[u],low[v]);
		else if (instack[v])
			low[u]=min(low[u],dfn[v]);
		p=e[p][1];
	}
	if (dfn[u]==low[u]){
		num++;
		do{
			val[num]++;
			p=s[s[0]--];
			col[p]=num;
			instack[p]=0;	
		}while (p!=u);
	}
}
int main(){	
	for (i=1;i<=n;i++) if (!vis[i]) dfs(i);/* 调用*/
}
/*
 调用之后 对与每一个点i col[i] 代表i所属的强连通分量的标号
*/