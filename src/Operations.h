//NOTES
//Change to std::list if time permits
//Think of new way to represent pivot , pivot = -1 initially is weird , and only works for integers anol
//Do insert function
//Change private , public later on

#include <bits/stdc++.h>
using namespace std;
#define NUM_LEVELS 5

template<typename T>
class DownBuffer
{
    public:
    vector<T> elements;
    int capacity;
    T pivot;
    DownBuffer *next;
    DownBuffer *prev;

    DownBuffer(int cap = 16)
    {
        next = nullptr;
        prev = nullptr;
        capacity = cap;
        pivot = 0;
    }

    void split(){
        sort(elements.begin(),elements.end());
        DownBuffer<T>* excess = new DownBuffer<T>(capacity);
        (this->next)->prev = excess;
        excess -> prev = this;
        this -> next = excess;

        for(int i = capacity/2 + 1 ; i <= capacity;i++){
            excess->insert(elements[i]);
        }
        for(int i = capacity/2 + 1; i <= capacity; i++){
            elements.pop_back();
        }
        pivot = 0;
        for(auto x : elements){
            if(x > pivot) pivot = x;
        }
    }

    void insert(T x){
        elements.push_back(x);
        if(pivot < x)pivot = x;
    }

    void print(){
        for(auto x : elements){
            cout<<x<<" ";
        }
        cout<<endl;
    }

    ostream& operator<<(ostream& os){
        for(auto x : elements){
            os<<x<<" ";
        }
        os<<endl;
        return os;
    }

    template<typename XXX> friend class Level;
    template<typename XXX> friend class Priority_Queue;
};

template<typename T>
class UpBuffer
{
    public:
    vector<T> elements;
    int capacity;

    UpBuffer(int cap = 16)
    {
        capacity = cap;
    }

    void insert(T x){
        elements.push_back(x);
    }

    void print(){
        for(auto x : elements){
            cout<<x<<" ";
        }
        cout<<endl;
    }

    template<typename S> friend class Level;
    template<typename XXX> friend class Priority_Queue;
};


template<typename T>
class Level
{
    public:
    UpBuffer<T> Up;
    DownBuffer<T>* head;
    DownBuffer<T>* tail;

    int Level_num; //size of the level/ up buffer
    int down_buffer_cap; //capacity of the downbuffers in this level
    int max_down_buffers;
    int cur_down_buffers;

    public:
    Level(double L_num = 16)
    {
        down_buffer_cap = 2*((int)pow(L_num,2/3)) - 1;
        max_down_buffers = pow(L_num,1/3);
        cur_down_buffers = 0;
        Level_num = L_num;
        head = new DownBuffer<T>;
        tail = new DownBuffer<T>;
        head -> next = tail;
        tail -> prev = head;

        Up = UpBuffer<T>(Level_num);
    }
    int size(){
        return Level_num;
    }

    template<typename XXX> friend class Priority_Queue;
    
};

template<typename T>
class Priority_Queue{
    public:
        Level<T> levels[NUM_LEVELS];
        int max_element_del_buf_index;
        vector<T> insertion_buffer;
        int ins_buf_cap;
        vector<T> deletion_buffer;
        int del_buf_cap;

    public:
        Priority_Queue(double c){
            for(int i = 0;i<NUM_LEVELS;i++){
                levels[i] = Level<T>((c+1e-5));
                c = pow(c , 1.5);
            }
            
            ins_buf_cap = del_buf_cap = pow(c,2/3);
            max_element_del_buf_index = -1;

        }
    //c is size of first level

    //INSERT FUNCTION IS NOT DONE AT ALL
        void insert(T to_insert){
            //If the insertion buffer is about to become bigger than insertion buffer capacity
            if(insertion_buffer.size()>=ins_buf_cap){
                push(0,insertion_buffer);
                insertion_buffer.clear();
            }

            //1)If deletion buffer itself is empty , this implies priority queue is also empty . As down buffer has smallest elements of pq. insert into deletion buffer .
            if(deletion_buffer.size() == 0){
                deletion_buffer.push_back(to_insert);
            }
            else{
                //FIND MAX element in the deletion buffer
                T max_index = 0;
                for(int i = 0 ; i < deletion_buffer.size() ; i++){
                    if(deletion_buffer[max_index] < deletion_buffer[i]) max_index = i ;
                }
                            
                if(deletion_buffer[max_index] < to_insert){
                    insertion_buffer.push_back(to_insert);
                }else{
                    insertion_buffer.push_back(deletion_buffer[max_index]);
                    deletion_buffer[max_index] = to_insert;
                }
            }

        }

        void push(int level_index,vector<T>& ins_elements){
            //1)Sort the elements to be inserted in this level
            sort(ins_elements.begin(),ins_elements.end(),greater<T>());


            //2)Sequentially fill downbuffers with elements less than the pivot . There may be some elements left after this.
            DownBuffer<T>* cur = (levels[level_index].head) -> next;
            while(cur != levels[level_index].tail && !ins_elements.empty()){
                while(!ins_elements.empty() && (cur -> pivot) > ins_elements.back()){
                    cur -> insert(ins_elements.back());
                    ins_elements.pop_back();
                    if((cur -> elements).size() > cur -> capacity){
                        cur -> split();
                        levels[level_index].cur_down_buffers++;
                    }
                }
                cur = cur -> next;
            }
            
            //3)In case the number of downbuffers exceeds max number of down buffers permitted for this level simply add these elements to ins_elements vector
            cur = levels[level_index].tail -> prev;
            while(levels[level_index].cur_down_buffers > levels[level_index].max_down_buffers){
                for(auto x : cur->elements){
                    ins_elements.push_back(x);
                }
                (cur -> next) -> prev = cur -> prev;
                (cur -> prev) -> next = cur -> next;
                delete cur;
                levels[level_index].cur_down_buffers--;
            }

            //4)In case ins_elements vector is not empty we must put these elements into the upbuffer
            UpBuffer<T>& up = levels[level_index].Up;
            while(!ins_elements.empty()){
                up.insert(ins_elements.back());
                ins_elements.pop_back();

                //5)????? If the up buffer exceeds its capacity we should push to the next level recursively.
                //Note that I have taken a decision of incorporating this IF STATEMENT within the while loop of ins_elements
                //I do not see any issue of keeping this IF STATEMENT completely outside the while loop after it
                //The change to be made would be that we must choose put the last (up.capacity) elements into vector next_level_push

                if((up.elements).size() > up.capacity){
                    vector<T> next_level_push;
                    for(int i = (up.elements).size()-1;i>=1;i--){
                        next_level_push.push_back((up.elements)[i]);
                        (up.elements).pop_back();
                    }
                    push(level_index + 1, next_level_push);
                }
            }

            return;
        }
};


