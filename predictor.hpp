#ifndef __PREDICTOR__
#define __PREDICTOR__

#include"counter2.hpp"
#include<cstdio>

class Predictor
{
    private:
        static const int N=3;
        Counter2 cnt[1<<N];
        bool buf[N];
    public:
        int tot;
        Predictor():tot(0) {}
        void push(bool cur)
        {
            for(int i=1;i<N;++i)
                buf[i-1]=buf[i];
            buf[N-1]=cur;
        }
        void update(int dir)
        {
            unsigned num=0;
            for(int i=0;i<N;++i)
                num=num<<1|buf[i];
            cnt[num].update(dir);
        }
        bool willJump()
        {
            ++tot;
            unsigned num=0;
            for(int i=0;i<N;++i)
                num=num<<1|buf[i];
            return cnt[num].willJump();
        }
};

#endif