const int                   Maxn=1200,oo=INT_MAX;
struct Node {
    int col,f,fl,wd,s[6];
    //col:颜色 f:父亲 fl:fail 指针 wd:是否是危险节点 s:儿子
};
struct AC_muto{
    int         cnt=0;
    Node        t[Maxn];
    int getcol(char p){
        //这里写 颜色函数 默认结束字符'\0'为-1 
        if (p=='\0') return -1;
        //除了'\0' 不要返回-1
    }
    int NewNode(int f,int col){
        t[++cnt].col=col; t[cnt].f=f;
        return cnt;
    }
    void ins(int k,char* p){
        int col=getcol(*p);
        if (col+1){
            if (!t[k].s[col]) t[k].s[col]=NewNode(k,col);
            ins(t[k].s[col],p+1);
        }
        else t[k].wd=1;
    }
    void make()
    {
        queue<int>              dl;
        dl.push(0);
        while (!dl.empty()){
            int u=dl.front();
            dl.pop();
            for (int i=1;i<=4;i++)
                if (t[u].s[i]){
                    int v=t[u].s[i],fl=t[u].fl;
                    dl.push(v);
                    while (fl &&!t[fl].s[i]) fl=t[v].fl;
                    t[v].fl=t[fl].s[i];
                    if (!t[v].f) t[v].fl=0;
                    t[v].wd|=t[u].wd||t[t[v].fl].wd;//危险标记的传递 类似一个dp的过程
                }
                else t[u].s[i]=t[t[u].fl].s[i];//加失配边
        }
    }
    int ACauto(char *){
        // 匹配过程
    }

};
// 调用 ins(1,char*) 在自动机内插入一个字符串
// 调用 make() 生成AC自动机
// 调用 ACauto(char* ) 对字符串进行匹配