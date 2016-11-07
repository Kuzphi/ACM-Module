int MinRe(char* S,int l)//下标从0开始
{
    int i=0,j=1,k=0,t;
    while(i<l&&j<l&&k<l)
    {
        t=S[(i+k)>=l?i+k-l:i+k]-S[(j+k)>=l?j+k-l:j+k];
        if (!t) k++;
        else{
            if (t>0) i+=k+1; else j+=k+1;
            j+=(i==j);
            k=0;
        }
    }
    return min(i,j);
}//返回最小表示的指针