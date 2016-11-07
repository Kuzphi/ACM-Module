#include <cmath>
#include <iostream>
#include <cstring>
using namespace std;
const int  Maxn = 300010, Maxm = 30;
struct PAM{
	struct Node{
		int nt[Maxm],fl,cnt,len;
	}node[Maxn];
	int last,n,cnt;
	char st[Maxn];
	int NewNode(int len){
		memset(node+cnt,0,sizeof(Node));
		node[cnt].len = len;
		return cnt++;
	}
	int Get_fl(int x){
		while(st[n - node[x].len - 1] != st[n]) x = node[x].fl;
		return x;
	}
	void ins(char ch , int p){
		st[++n] = ch;
		int c = ch - 'a';
		int cur = Get_fl(last);
		if (!node[cur].nt[c]){
			int now = NewNode(node[cur].len + 2);
			node[now].fl = node[Get_fl(node[cur].fl)].nt[c];
			node[cur].nt[c] = now;
		}
		last = node[cur].nt[c];
		node[last].cnt ++;
	}
	void initialization(){
		st[0] = '$';
		cnt = n = last = 0;
		NewNode(0);
		NewNode(-1);
		node[0].fl = node[1].fl = 1;
	}
	void build(char* s){ //1-index
		initialization();
		int len = strlen(s + 1);
		for (int i=1;i<=len;i++) ins(s[i], i);
		for (int i = cnt - 1; i >= 2;i--){
			node[node[i].fl].cnt += node[i].cnt;
			// for (int j=i-node[i].len;j<=i-1;j++) cout << st[j];//cout << endl;
			// cout <<" "<< node[i].cnt << endl;
		}
	}
}PAM;