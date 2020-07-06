#include<cstring>

#ifndef _REGISTER_
#define _REGISTER_
class Register
{
    friend class Execute;
    private:
        unsigned data[32];
        unsigned pc;
    public:
        Register()
        {
            std::memset(data,0,sizeof(data));
            pc=0;
        }
};
#endif