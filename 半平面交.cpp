#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;

const int Maxn=4010,oo=100000000;
const double eps=1e-16;
//半平面为向量表示，平面位于向量左侧
//时间效率nlogn，瓶颈在与排序算法，使用基数排序可优化到线性
struct P{double x,y;}           cx[Maxn];//point
struct L{P a,b;double ang;}     l[Maxn],st[Maxn];//h-plane
int                             n,m,tmp;

double operator * (P x,P y)
{
    return x.x*y.y-x.y*y.x;
}//x-multi
P operator - (P x,P y)
{
    x.x-=y.x;x.y-=y.y;
    return x;
}//make vector
P operator * (L x,L y)
{
    double a1=(y.b-x.a)*(y.a-x.a),a2=(y.a-x.b)*(y.b-x.b);
    P r;
    r.x=(x.a.x*a2+x.b.x*a1)/(a2+a1);
    r.y=(x.a.y*a2+x.b.y*a1)/(a2+a1);
    return r;
}//get cross point
bool operator == (P a,P b)
{
    return fabs(a.x-b.x)<eps&&fabs(a.y-b.y)<eps;
}//check two equal point
bool operator < (L x,L y)
{
    if(fabs(x.ang-y.ang)<eps)
        return (y.b-x.a)*(x.b-y.a)>eps;
    return x.ang<y.ang;
}//smaller h-plane
bool judgeout(L x,P p0)
{
    return (p0-x.a)*(x.b-x.a)>eps;
}//check the new h-plane and the cross point in two-end stack
bool pall(L x,L y)
{
    return fabs((x.b-x.a)*(y.b-y.a))<eps;
}//check parallel
void addline(double x1,double y1,double x2,double y2){//按向量添加半平面
    l[tmp].a.x=x1;  l[tmp].a.y=y1;
    l[tmp].b.x=x2;  l[tmp].b.y=y2;
    l[tmp].ang=atan2(l[tmp].b.y-l[tmp].a.y,l[tmp].b.x-l[tmp].a.x);
    tmp++;
}
bool addline(double a, double b, double c)//按ax+by+c<0的方程 添加半平面
{
    if (fabs(a)<eps&&fabs(b)<eps) return c<0;
    else if (fabs(a)<eps)   addline(b<0?0:1 ,-c/b, b<0?1:0 , -c/b);
    else if (fabs(b)<eps)   addline(-c/a, a<0?1:0, -c/a, a<0?0:1);
    else if (b>0)       addline(1,-(a+c)/b,0,-c/b);
    else                addline(0,-c/b,1,-(a+c)/b);
    return 1;
}
bool hplane_intersection()//返回0表示交为空集
{
    m=0;
    n=tmp;
    int top=1,bot=0;
    sort(l,l+n);
    int tmp=1;
    for(int i=1;i<n;++i)
        if(l[i].ang-l[i-1].ang>eps)l[tmp++]=l[i];
    n=tmp;
    st[0]=l[0],st[1]=l[1];
    for(int i=2;i<n;++i)
    {
        if(pall(st[top],st[top-1])||pall(st[bot],st[bot+1]))return 0;
        while(bot<top&&judgeout(l[i],st[top]*st[top-1]))top--;
        while(bot<top&&judgeout(l[i],st[bot]*st[bot+1]))bot++;
        st[++top]=l[i];
    }
    while(bot<top&&judgeout(st[bot],st[top]*st[top-1]))top--;
    while(bot<top&&judgeout(st[top],st[bot]*st[bot+1]))bot++;
    if (top<=bot+1) return 0;
    st[++top]=st[bot];
    for(int i=bot;i<top;++i)
        cx[m++]=st[i]*st[i+1];
    return 1;
}
double Area()
{
    cx[m]=cx[0];
    double ans=0;
    for(int i=0;i<m;++i)
        ans+=cx[i]*cx[i+1];
    return fabs(ans)/2;
}
void init()
{
    tmp=0;
    addline(0,oo,0,0);
    addline(0,0,oo,0);
    addline(oo,0,oo,oo);
    addline(oo,oo,0,oo);
}
int main()
{
    double w[Maxn],u[Maxn],v[Maxn];
    int num;
    while(~scanf("%d",&num))
    {
        for(int i=0;i<num;i++) scanf("%lf%lf%lf",&v[i],&u[i],&w[i]);
        for(int i=0;i<num;i++)
        {
            init();
            int ck=1;
            for (int j=0;j<num;j++) if (i-j)
            {
                double a=(v[j]-v[i])/(v[i]*v[j]);
                double b=(u[j]-u[i])/(u[i]*u[j]);
                double c=(w[j]-w[i])/(w[i]*w[j]);
                ck=ck&&addline(a, b, c);
            }
            hplane_intersection();
            if (Area()>eps&&ck) puts("Yes"); else puts("No");
        }
    }
    return 0;
}