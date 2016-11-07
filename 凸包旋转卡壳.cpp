#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <math.h>
using namespace std;
#define PB push_back
#define CLR(x) memset(x,0,sizeof(x))
#define ll long long 
#define oo 210000000
#define Maxn 50010
#define pi acos(-1.0)
struct Vec{//既可以看成向量又可以看成点
    int x,y;
    Vec(int x,int y):x(x),y(y){}
    Vec(){}
};
Vec operator - (Vec a,Vec b){ return Vec(a.x-b.x,a.y-b.y); } //向量作差 或者 向量(b->a)
int operator < (Vec a,Vec b){ return a.x==b.x? a.y<b.y:a.x<b.x; } //x为第一关键字，y为第二关键字比较大小
int dot(Vec a,Vec b) {return a.x*a.y+b.x*b.y;} //向量点积
int det(Vec a,Vec b) {return a.x*b.y-a.y*b.x;} //向量叉积
int det(Vec a,Vec b,Vec c){return det(b-a,c-a);} //三个点的叉积 (b-a) 叉积 (c-a)
int dis(Vec a){return (a.x*a.x+a.y*a.y);} //向量的模
int dis(Vec a,Vec b) {return dis(a-b);}   //两点间距离


int n,T;
Vec p[Maxn],cov[Maxn];

int cmp(Vec a,Vec b)//极角比较 注意p[1]应该定义在全局变量
{
    int k=det(p[1],a,b);
    if (k) return k>0;
    return a<b;
}
int Graham(int n,Vec p[],Vec sk[])
{   
    //做凸包 sk[]为最终答案，视情况选择sk中放编号还是放点,返回凸包上点的数量
    for (int i=2;i<=n;i++)
    if (p[i]<p[1]) 
        swap(p[i],p[1]); //选最左最下的点
    sort(p+2,p+n+1,cmp); //极角排序
    int h=2;
    sk[1]=p[1]; sk[2]=p[2]; //注意一共只有一个点的情况 此处未处理该情况
    for (int i=3;i<=n;i++){
        while(h-1 && det(sk[h-1],sk[h],p[i])<=0 ) h--;//<=0 去掉共线点 <0保留共线点
        sk[++h]=p[i];
    }
    return h;
}
double Rotating_Calipers(int n,Vec cov[])
{
    //旋转卡壳 n点数量 cov每个点坐标 逆时针顺序
    cov[n+1]=cov[1];
    int ans=0;
    for (int x=1,y=2;x<=n;x++)
    {
        while(det(cov[x],cov[x+1],cov[y+1]) > det(cov[x],cov[x+1],cov[y])) y=y%n+1;
        ans=max(ans, dis(cov[x+1],cov[y+1]) );//两线平行
        ans=max(ans,max( dis(cov[x],cov[y]) , dis(cov[x+1],cov[y]) ) );
    }
    return ans;
}
int main()
{
    while(~scanf("%d",&n))
    {        
        for (int i=1;i<=n;i++) 
                scanf("%d%d",&p[i].x,&p[i].y);
        int m=Graham(n,p,cov);
        cov[m+1]=cov[1];
        int ans=Rotating_Calipers(m,cov);
        printf("%d\n",ans);
    }
}