#include<queue>
#include<iostream>
#include<vector>
#include<set>
using namespace std;
deque<int> Q;
set<int> S;
// struct cmp
// {
//     bool operator() (const int &a,const int &b)
//     {
//         return a>b;
//     }
// };
// priority_queue<int,vector<int>,cmp> Q;
int main()
{
    // for(int i=1;i<=3;++i) Q.push(i);
    // cout<<Q.top()<<endl;
    Q.push_back(1);
    Q.push_back(2);
    Q.push_back(3);
    // for(int i=1;i<=3;++i) S.insert(i);
    deque<int>::iterator it;
    int *ptr;
    // it=Q.begin();
    it=Q.begin(),++it;
    ++it;
    ptr=&(*it);
    cout<<*ptr<<endl;
    // Q.pop_front();
    // Q.erase(it);
    *it=4;
    // Q.erase(1);
    cout<<*ptr<<endl;
    return 0;
}