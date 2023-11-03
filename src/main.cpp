#include<bits/stdc++.h>
#include"Operations.h"
#define info 0
#define NUM_LEVELS 5
using namespace std;

int main(){
    cout<<"running"<<"\n";
    Priority_Queue<int> pq(3,NUM_LEVELS);
    vector<int> v;
    
    for(int i = 2 ;i<100;i++)
        v.push_back(i);
    
    pq.push(0,v);
    pq.deletion_buffer.push_back(1);

    for(int i = 0; i<50;i++){
        cout<<pq.delete_min()<<"\n";
    }

    if(info == 1){
        for(int i = 0;i<NUM_LEVELS;i++){
            cout<<"size of "<<i<<"th level is :"<<pq.levels[i].Level_num<<"\n";
            cout<<"min size of "<<i<<"th level down buffer is(except first) :"<<pq.levels[i].down_buffer_min<<"\n";
        }
    }

    pq.print();





}

/*pull checker
    vector<int> temp;
    pq.pull(0,temp);
    for(auto x : temp)cout<<x;
    end pull checker
*/