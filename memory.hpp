#include<cstdio>
#include<iostream>
#include<cstring>

#ifndef _MEMORY_
#define _MEMORY_

class Memory
{
    friend class Execute;
    private:
        // static const int maxN=100;
        static const int maxN=1<<20;
        unsigned data[maxN+50],cnt;
        char seq[20];
        char get()
        {
            char ch=getchar();
            while ((~ch)&&ch!='@'&&!isdigit(ch)&&!isupper(ch)) ch=getchar();
            return ch;
        }
        unsigned seq2int(char *seq)   //base 16
        {
            int len=std::strlen(seq);
            unsigned ret=0;
            for(int i=0;i<len;++i)
                ret=ret*16+(unsigned)(seq[i]>='A'?seq[i]-'A'+10:seq[i]-'0');
            return ret;
        }
    public:
        Memory()
        {
            freopen("in","r",stdin);
            cnt=0;
            std::memset(data,0,sizeof(data));
            std::memset(seq,0,sizeof(seq));
        }
        Memory(const char *fname)
        {
            freopen(fname,"r",stdin);
            cnt=0;
            std::memset(data,0,sizeof(data));
            std::memset(seq,0,sizeof(seq));
        }
        void init_read()
        {
            char ch=getchar();
            while (~ch)
            {
                if (ch=='@')
                {
                    for(int i=7;~i;--i) seq[i]=get();
                    cnt=seq2int(seq);
                }
                else
                {
                    seq[0]=ch,seq[1]=get(),seq[2]=0;
                    data[cnt++]=seq2int(seq);
                } 
                ch=get();
            }
        }
        unsigned fetch(unsigned pc)
        {
            return data[pc]+(data[pc+1]<<8)+(data[pc+2]<<16)+(data[pc+3]<<24);
        }
        void set(unsigned pos,unsigned val)
        {
            data[pos]=val;
        }
};

#endif