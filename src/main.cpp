#include<bits/stdc++.h>
#include"Operations.h"
#define info 0
#define NUM_LEVELS 10
using namespace std;

int main(){
    cout<<"running"<<"\n";
    Priority_Queue<int> pq(3,NUM_LEVELS);
    vector<int> v;
    
    for(int i = 1 ;i<8;i++){    
        int x;
        cin>>x;
        pq.insert(x);
    }

    cout<<"least element:"<<pq.delete_min();

    if(info == 1){
        for(int i = 0;i<NUM_LEVELS;i++){
            cout<<"size of "<<i<<"th level is :"<<pq.levels[i].Level_num<<"\n";
            cout<<"min size of "<<i<<"th level down buffer is(except first) :"<<pq.levels[i].down_buffer_min<<"\n";
        }
    }

    //pq.print();

}

/*pull checker
    vector<int> temp;
    pq.pull(0,temp);
    for(auto x : temp)cout<<x;
    end pull checker
*/