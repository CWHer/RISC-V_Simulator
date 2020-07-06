#include<cstdio>
#include<cstring>
#include<cstdlib>
using namespace std;
char str[100];
int F(char ch)
{
    return ch<='9'?ch-'0':ch-'A'+10;
}
int main()
{
    while (true)
    {
        scanf("%s",str);
        int len=strlen(str);
        for(int i=0;i<len;++i)
        {
            char s[10];
            int x=F(str[i]);
            // printf("%b",x);
            // itoa(x,s,2);
            printf("%04s",s);
        }
        putchar('\n');
    }
    return 0;
}