#include<cstring>
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
        }

};