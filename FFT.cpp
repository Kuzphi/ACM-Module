// FFT:
#include <complex>
#define ll long long
using namespace std;
const int                   Maxn=2100000;
const double                pi=acos(-1.0);
typedef complex<double>     cp;
//n,nA,nB都是数组长度,即最高次是x^(n-1)

//复数的FFT,若TLE可改为两个数组模拟实部虚部,或结构体来实现
namespace FFT{
    struct Complex {
        double a,b;
        Complex() {}
        Complex(double a,double b):a(a),b(b) {}
    };
    inline Complex operator+(Complex &a, Complex &b) { return Complex(a.a+b.a,a.b+b.b);}
    inline Complex operator-(Complex &a, Complex &b) { return Complex(a.a-b.a,a.b-b.b);}
    inline Complex operator*(Complex &a, Complex &b) { return Complex(a.a*b.a-a.b*b.b,a.b*b.a+a.a*b.b);}

    Complex FA[Maxn],FB[Maxn],FC[Maxn],tmp[Maxn],iExp[Maxn] , Exp[Maxn];
    int W = 1 << 20;
    int Getlen(int x){ while(x&(x-1)) x+=x&-x; return x;}
    void pre(){        
        for (int i=0;i<W;i++){
            double x = i * 2 * pi / W;
            Exp[i]  = Complex(cos(x) , sin(x));
            iExp[i] = Complex(cos(x) , -sin(x));
        }
    }
    void Fft(int n,Complex* f,bool inv) {
        for (int i = 1 , j = 0; i < n; i++){
            int bit = n >> 1;
            while(j >= bit) j-=bit , bit >>= 1;
            j += bit;
            if (i < j) swap(f[i],f[j]);
        }
        for (int len = 2, s = W >> 1;len <= n;len <<= 1 , s >>= 1){
            int xlen = len >> 1;
            for (int i=0;i<n;i+=len){
                for (int j= 0;j< xlen ;j++){
                    Complex u = f[i + j];
                    Complex v = f[i + j + xlen] * (inv ? iExp[j * s] : Exp[j * s]);
                    f[i + j] = u + v;
                    f[i + j + xlen] = u - v;
                }
            }
        }
        if (inv) for (int j=0;j<n;j++) f[j].a/=n;
    }

    int Poly_Mul(int * A,int* B,int* C,int nA,int nB){
    //A B:多项式的系数 nA,nB:A B数组长度 C:最后答案
        pre();
        int nC = nA + nB - 1 , m = Getlen(nC);
        for (int i=0;i<m;i++) FA[i]=Complex(A[i],0);
        for (int i=0;i<m;i++) FB[i]=Complex(B[i],0);    
        Fft(m,FA,0); Fft(m,FB,0); 
        for (int i=0;i<m;i++) FC[i]=FA[i]*FB[i];
        Fft(m,FC,1); 
        for (int i=0;i<m;i++) C[i] = FC[i].a;
        return nC;
    }
};
//整数FFT
int Mo = 998244353; //系数的模
/*
P:FFT的模,G: P的原根,注意P应大于系数可能出现的最大值
P=C*2^k+1 注意选择合适的P 这里是119*2^23+1 = 1004535809
注意1.FA FB可能引起重复调用 2.如果要像Inv一样直接调用FFT,每次在第t项之后要清零
*/
int P  = 998244353 , G = 3;
struct Polynomial{    
    ll tA[Maxn],tB[Maxn],tmp[Maxn],sA[Maxn],Rev[Maxn];
    ll pw (ll a, int b,int Mo){
        ll ans = 1;
        for (;b;b/=2,a=a*a%Mo)
            if (b&1) ans =  a * ans  % Mo; 
        return ans;
    }
    int Getlen(int x){ while(x&(x-1)) x+=x&-x; return x;}
    //n:长度(2的幂),A:系数数组,ck 0是FFT,1是DFT
    void FFT(int N,ll* A,int ck){
        ll w1 = pw(G,(P-1)/N,P) , Rev = pw(N,P-2,P) ,Wx[Maxn];
        Wx[0] = 1;
        for (int i=1;i<=N;i++) Wx[i] = w1 * Wx[i-1] % P;
        for (int d=N/2; d ;d/=2){
            ll wn = 1 , w = ck?Wx[N-d]:Wx[d];
            for (int i=0; i<N; i+=d,wn=wn*w%P)
                for (int j=0, p=2*i%N; j<d ;j++)
                    tmp[i+j]= ( wn * A[p+d+j] + A[p+j])%P;
            for (int i=0;i<N;i++) A[i]=tmp[i];
        }
        if (ck) for (int i=0;i<N;i++) A[i]= Rev * A[i] % Mo;
    } 
    int Mul(ll * A,ll* B,ll* C,int nA,int nB){        
        int nC = nA+nB-1 , m = Getlen(nC);
        for (int i=0;i<m;i++)  tA[i]=tB[i]=0;
        for (int i=0;i<nA;i++) tA[i]=A[i];
        for (int i=0;i<nB;i++) tB[i]=B[i];            
        FFT(m,tA,0); FFT(m,tB,0);
        for (int i=0;i<m;i++) C[i] = tA[i] * tB[i] % P;
        FFT(m,C,1);
        return nC;
    }
    void Div(ll* A,ll* B,ll *D,ll *R,int nA,int nB){
    //多项式A=B*D+R 求得D,R      
        reverse(A,A+nA); reverse(B,B+nB);
        int nD = nA - nB + 1, nR = nB - 1;
        Inv(nD,B,D);
        Mul(A,D,D,nD,nD);
        Mul(B,D,R,nB,nD);
        for (int i=0;i<nR;i++) R[i]=(A[i+nD]-R[i+nD])%Mo;
        reverse(A,A+nA); reverse(B,B+nB);
        reverse(D,D+nD); reverse(R,R+nR);
    }
    void Inv(int t,ll *A,ll *B){
    //多项式A在模x^t意义下的逆元B
        memset(sA,0,sizeof(sA));
        for(int i=0;i<t;i++) sA[i]=A[i]; 
        B[0] = pw(A[0],Mo-2,Mo); t*=2;
        for (int m=2;m<t;m<<=1){          
            for(int i=m/2;i<m*2;i++) B[i] = tA[i] = 0;
            for(int i=0;i< m;i++) tA[i] = sA[i];
            FFT(m*2,tA,0);FFT(m*2,B,0);
            for(int i=0;i<m*2;i++) B[i]=(2 - tA[i]*B[i]) %P * B[i] % P;
            FFT(m*2,B,1);
        }
    }
    void Sqrt(int t,ll *A,ll *B){
    //B^2=A % x^t 注意B的长度nB = t; 
    //仅处理A[0]=1的情况,不然用BGstep求A[0]对Mo的二次剩余
        int Rev2 = pw(2,Mo-2,Mo);
        B[0] = 1; t<<=1;
        for (int m=2;m<t;m<<=1){
            Inv(m,B,Rev);
            Mul(A,Rev,Rev,m,m);
            for (int i=0;i<m;i++) B[i] = (B[i]+Rev[i])%P * Rev2 % Mo;
        }
    }
}Poly;
int N,M;
ll  C;
ll  A[Maxn],D[Maxn],R[Maxn],ANS[Maxn],F[Maxn],Now[Maxn];
ll Solve(ll N, ll M, ll *A , ll *F){
    memset(ANS,0,sizeof(ANS));
    memset(Now,0,sizeof(Now));
    ANS[0]=Now[1]=1;
    for(;N>0;N>>=1){
        if (N%2) {
            Poly.Mul(ANS,Now,ANS,M,M);
            Poly.Div(ANS,A,D,R,M+M-1,M+1);
            for (int i=0;i<M;i++) ANS[i] = R[i];
            for (int i=M;i<M+M;i++) ANS[i] = 0;   
        }
        Poly.Mul(Now,Now,Now,M,M);        
        Poly.Div(Now,A,D,R,M+M-1,M+1);
        for (int i=0;i<M;i++) Now[i] = R[i];
        for (int i=M;i<M+M;i++) Now[i] = 0;            
    }
    ll ans = 0;
    for (int i=1;i<=M;i++) ans = (ans + ANS[i] * F[i]) % Mo;
    return ans;
}

struct XOR_FFT{
    void Xor_fft(int l, int r , double* A){
    if(l == r) return;
    int mid = (r + l) >> 1; 
    Xor_fft(l , mid , A);
    Xor_fft(mid + 1, r  , A);
    mid = (r - l + 1) >> 1;
    for (int i=0;i<mid;i++) {
        double x = A[l + i] , y = A[l + i + mid]; 
        A[l + i] = x - y;
        A[l + mid + i] = x + y;
    }       
}

void Xor_dft(int l,int r,double * A){
    if (l == r) return;
    int mid = (r - l + 1) >> 1;
    for (int i=0;i<mid;i++) {
        double x = A[l + i] , y = A[l + i + mid]; 
        A[l + i] = (y + x) / 2;
        A[l + i + mid] = (y - x) / 2;
    }   
    mid = (l + r) >> 1;
    Xor_dft(l , mid , A);
    Xor_dft(mid + 1 , r , A);
}
};


int main(){
    scanf("%d%d",&N,&M); 
    cin>>C; F[M+1]=C;
    for (int i=1;i<=M;i++) cin>>A[i];
    for (int i=1;i<=M;i++) cin>>F[i];
    for (int i=1;i<=M;i++) F[M+1] = (F[M+1] + F[M+1-i] * A[M-i+1])%Mo;
    A[M+1] = -A[M] - 1;
    A[M+2] = 1;
    for (int i=M;i>=1;i--) A[i]-=A[i-1];
    M+=2;
    ll ans = Solve(N,M,A,F);
    cout << (ans+Mo)%Mo <<endl;

}