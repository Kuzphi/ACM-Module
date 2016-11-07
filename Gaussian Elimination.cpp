#include <iostream>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <vector>
//系数矩阵size N*M,R:矩阵的秩,返回0表示无解,mat[1][M+1]为最后答案
bool Xor_Gauss_Eliminate(int N,int M,int& R){
    R = 0;
    for (int C=1;R<=N && C<=M;C++){
        int x = R + 1;
        for (;x<=N;x++) if (mat[x][C]) break;
        if (x==N+1) continue;  R++;
        for (int i=C;i<=M+1;i++) swap(mat[R][i],mat[x][i]);
        for (int i=1;i<=N;i++)
            for (int j=M+1;i-R && mat[i][C];j--)
                mat[i][j]^=mat[R][j];
    }
    for (int i=R+1;i<=N;i++) if (mat[i][M+1]) return 0;
    return 1;
}
bool Int_Gauss_Eliminate(int N,int M,int& R,int Mo){
    R = 0;
    for (int C=1;C<=M && R<=N ;C++){
        int x = R + 1;
        for (;x<=N;x++) if (mat[x][C]) break;
        if (x==N+1) continue;  R++;
        for (int i=C;i<=M+1;i++) swap(mat[R][i],mat[x][i]);
        int Rev = Pow(mat[R][C],Mo-2,Mo);
        for (int i=C;i<=M+1;i++) mat[R][i] = mat[R][i]* Rev % Mo;
        for (int i=1;i<=N;i++)
            for (int j=M+1;i-R && mat[i][C];j--)
                mat[i][j] = ( (mat[i][j] - mat[R][j] * mat[i][C]) % Mo + Mo) %Mo;
    }
    for (int i=R+1;i<=N;i++) if (mat[i][M+1]) return 0;
    return 1;
}
bool Real_Gauss_Eliminate(int N,int M,int& R){
//系数矩阵size N*M ,R:系数矩阵的秩,返回0表示无解
    R = 0;
    for (int C=1;C<=M && R<=N ;C++){
        int x = R + 1;
        for (int i=x;i<=N;i++) if (fabs(mat[i][C]) > fabs(mat[x][C])) x = i;
        if (fabs(mat[x][C])<eps) continue;  R++;
        for (int i=C;i<=M+1;i++) swap(mat[R][i],mat[x][i]);
        for (int i=M+1;i>=C;i--) mat[R][i] /= mat[R][C];
        for (int i=1;i<=N;i++)
            for (int j=M+1;i-R && j>=C;j--)
                mat[i][j] -= mat[i][C] * mat[R][j];
    }
    for (int i=R+1;i<=N;i++) if (fabs(mat[i][M+1])>eps) return 0;
    return 1;
}
