#include<queue>
#include<iostream>
using namespace std;
deque<int> Q;
priority_queue<int> Q;
int main()
{
    Q.push_back(1);
    Q.push_back(2);
    Q.push_back(3);
    deque<int>::iterator it;
    int *ptr;
    it=Q.begin();
    ptr=&Q[1];
    cout<<*ptr<<endl;
    Q.pop_front();
    cout<<*ptr<<endl;
    return 0;
}