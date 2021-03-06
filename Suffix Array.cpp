#include <iostream>
#include <math.h>
#include <vector>
#include <cstring>
#include <cstdio>
#include <algorithm>
#define CLR(x) memset(x,0,sizeof(x))
#define For(i,x,y) for(int i=x;i<=y;i++)
#define CMP(a,b,d) (rk[a]!=rk[b] || rk[a+d]!=rk[b+d])
using namespace std;
const int                 Maxn=200010;
struct Suffix_Array{
    int n,*s,m;
    int rk[Maxn],sa[Maxn],h[Maxn];
    int cnt[Maxn],tmp[Maxn],f[Maxn][20];
    //注意数组要开两倍以上
    void Init(int xn,int *xs,int xm){n=xn,s=xs,m=xm;}
    void Sort(int d){
        CLR(cnt);
        For(i,1,n) cnt[rk[i+d]]++;//统计rank[i]的个数
        For(i,1,n) cnt[i]+=cnt[i-1];
        for (int i=n;i>=1;i--) tmp[cnt[rk[sa[i]+d]]--]=sa[i];
        memcpy(sa,tmp,(n+1)*sizeof(int));
    }
    void Array(){
//把字符串转换为整数再传入，数组下标要从1开始，m为数组中最大的值 s[n+1]也必须是分隔符
        CLR(cnt); CLR(rk);
        For(i,1,n) cnt[s[i]] = 1;
        For(i,0,m) cnt[i] += cnt[i-1];
        For(i,1,n) rk[i] = cnt[s[i]],sa[i] = i;
        for (int d=1;d<=n;d*=2){
            Sort(d);Sort(0);
            For(i,1,n)
                tmp[sa[i]]=tmp[sa[i-1]]+CMP(sa[i],sa[i-1],d);
            memcpy(rk,tmp,(n+1)*sizeof(int));
            if (rk[sa[n]]==n) break;//优化
        }       
    }
    void Height(){//计算Height数组
        CLR(h);
        For(i,1,n)
        if (rk[i]!=n){
            int p=rk[i],nt=sa[p+1];
            while(s[h[p]+i]==s[h[p]+nt]) h[p]++;
            if (h[p]) h[rk[i+1]]=h[p]-1;
        }
        int x=n-1,p= log(x*1.0) / log(2.0);
        For(i,1,x) f[i][0]=h[i];    
        For(j,1,p) For(i,1,x-(1<<j)+1)
            f[i][j]=min(f[i][j-1],f[i+(1<<(j-1))][j-1]);
    }   
    int LCP(int x,int y){//第x个后缀和第y个后缀求LCP
        if (x==y) return n-x+1;
        x=rk[x],y=rk[y];
        int l=min(x,y),r=max(x,y)-1;
        int p= log(r-l+1.0) / log(2.0);
        return min( f[l][p],f[r-(1<<p)+1][p]);
    }
}SA;
//Init传入字符串长度和数组指针，调用Array计算rk和sa数组，调用height计算height数组，再调用LCP

