int Manacher(char* S)
{
    memset(len,0,sizeof(len));
    memset(T,0,sizeof(T));
    int l=strlen(S+1);
    For(i,1,l) T[2*i]=S[i],T[2*i+1]='#';
    T[0]='$'; T[1]='#';
    l=l*2+1;
    id=Mlen=ans=0;
    For(i,1,l)
    {
        len[i]=1;
        if (Mlen>i) len[i]=min(len[2*id-i],Mlen-i);//最小值
        while(T[i+len[i]]==T[i-len[i]]) len[i]++;
        if (i+len[i]>Mlen)
            Mlen=i+len[i],id=i;
    }
    For(i,1,l) ans=max(ans,len[i]-1);//len[i]-1 就是以i为中心的最长回文子串 
}