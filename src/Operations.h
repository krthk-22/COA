//NOTES
//Change to std::list if time permits
//Think of new way to represent pivot , pivot = -1 initially is weird , and only works for integers anol
//Do insert function
//Change private , public later on
//NO edge case condition added when pushing to last level , pulling from last level

//PULL OPERATION UP BUFFER , can it have some less number of elements than initial when pulling ?????? altho this means that we cannot pull unless we do global reshuffling 
//MAKE a call for above statement , what to do ?

#include <bits/stdc++.h>
#include "Overload.h"
using namespace std;

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
    }

    void split(){
        sort(elements.begin(),elements.end());
        DownBuffer<T>* excess = new DownBuffer<T>(capacity);
        (this->next)->prev = excess;
        excess -> next = this -> next;
        excess -> prev = this;
        this -> next = excess;

        for(int i = capacity/2 + 1 ; i <= capacity;i++){
            excess->insert(elements[i]);
        }
        for(int i = capacity/2 + 1; i <= capacity; i++){
            elements.pop_back();
        }
        
        recalc_pivot();
        excess -> recalc_pivot();
    }

    void insert(T x){
        elements.push_back(x);
        if(elements.size() == 1){
            pivot = x;
        }
        else{
            if(pivot < x)pivot = x;
        }    
    }

    void recalc_pivot(){
        if(elements.size()==0)return;
        else{
            T temp_pivot = elements[0];
            for(auto x : elements){
                if(temp_pivot < x)temp_pivot = x;
            }
            pivot = temp_pivot;
        }   
    }

    void print(){
        for(auto x : elements){
            cout<<x<<" ";
        }
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
    int down_buffer_min; //Every downbuffer after the first one should be minimum of this length
    int max_down_buffers;
    int cur_down_buffers;

    public:
    Level(double L_num = 16)
    {
        down_buffer_cap = 2*((int)pow(L_num,2.0/3.0)) - 1; 
        down_buffer_min = pow(L_num,2.0/3.0);
        max_down_buffers = pow(L_num,1.0/3.0) + 2; //This is ceil(pow(L_num,1.0/3.0)) + 1 when the inner thing is not an integer
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
    void print(){
        cout<<"Level_num(size) : "<<Level_num<<"| Upbuffer: ";
        Up.print(); //Contains an internal newline
        DownBuffer<T>* cur = head -> next;
        cout<<"Downbuffers : ";
        while(cur != tail){
            cur->print();//Does not print newline internally
            if(cur != tail ->prev )cout<<" | ";
            cur = cur -> next;
        }
        cout<<"\n";
    }

    template<typename XXX> friend class Priority_Queue;
    
};

template<typename T>
class Priority_Queue{
    public:
        Level<T>* levels;
        int max_element_del_buf_index;
        vector<T> insertion_buffer;
        int ins_buf_cap;
        vector<T> deletion_buffer;
        int del_buf_cap;
        int total_elements;
        int NUM_LEVELS;

    public:
        Priority_Queue(double c,int NUM_LEVELS){
            this -> NUM_LEVELS = NUM_LEVELS;
            total_elements = 0;
            ins_buf_cap = del_buf_cap = pow(c,2.0/3.0);
            levels = new Level<T>[NUM_LEVELS];
            for(int i = 0;i<NUM_LEVELS;i++){
                levels[i] = Level<T>((c+1e-5)); 
                c = pow(c , 1.5);
            }
            
            max_element_del_buf_index = -1;

        }
    //c is size of first level

        void print(){
            cout<<"deletion buffer:";
            for(auto x : deletion_buffer)cout<<x<<" ";
            cout<<endl;
            cout<<"insertion buffer:";
            for(auto x : insertion_buffer)cout<<x<<" ";
            cout<<endl;
            for(int i = 0;i < NUM_LEVELS ;i++){
                levels[i].print();
            }
        }

    //INSERT FUNCTION IS NOT DONE AT ALL
        void insert(T to_insert){
            //1)If deletion buffer itself is empty , this implies priority queue is also empty . As down buffer has smallest elements of pq. insert into deletion buffer .
            if(deletion_buffer.size() == 0){
                deletion_buffer.push_back(to_insert);
                return;
            }
            else{
                //FIND MAX element in the deletion buffer
                int max_index = 0;
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

            //3)If the insertion buffer is about to became bigger than insertion buffer capacity
            if(insertion_buffer.size()>=ins_buf_cap){
                push(0,insertion_buffer);
                insertion_buffer.clear();
            }

        }

        bool empty(){
            return deletion_buffer.empty();
        }

        T top(){
            if(deletion_buffer.size() > 0)
            return deletion_buffer[0];
            else {
                T garbage;
                return garbage;
            }
        }

        T delete_min(){
            //0)If deletion buffer is empty it implies there are no elements in whole priority queue
            if(deletion_buffer.empty()){
                T garbage;
                return garbage;
            }
            //1)Find min_element in the down buffer 
            //Do note that this will be possible if there are atleast 1 element in the whole priority queue
            //At end of every delete min we make sure that deletion buffer is not empty (pull elements from priority queue levels+ins_buffer if there are atleast 1 element in all that then good)
            
            auto min_element_itr = deletion_buffer.begin();
            T min_element;
            
            for(auto itr = deletion_buffer.begin(); itr != deletion_buffer.end(); itr++){
                if(*itr < *min_element_itr) min_element_itr = itr;
            }
            min_element = *min_element_itr;
            deletion_buffer.erase(min_element_itr);

            if(deletion_buffer.empty()){
                //2)If in case the deletion buffer becomes empty now , then we pull the smallest c^2/3 elements from level 0 AND we combine this with the elements in insertion buffer. And clear the insertion buffer .
                vector<T> deletion_buffer_probables;
                pull(0,deletion_buffer_probables);
                deletion_buffer_probables.insert(deletion_buffer_probables.end(),insertion_buffer.begin(),insertion_buffer.end());
                insertion_buffer.clear();

                //3)Sort these probables
                sort(deletion_buffer_probables.begin(),deletion_buffer_probables.end(),greater<T>());

                //4)Fill the smallest deletion_buffer_cap elements into deletion buffer . IF the probables vector has enough elements that is lol . 
                while(!deletion_buffer_probables.empty() && deletion_buffer.size() < del_buf_cap){
                    deletion_buffer.push_back(deletion_buffer_probables.back());
                    deletion_buffer_probables.pop_back();
                }
            
                //5)Insert remaining elements into the insertion buffer . In non-edge cases the insertion buffer will end up with the same number of elements that it started with . 
                //Only when the pull operation does not give full del_buf_cap number of elements then some number of elements from insertion buffer will decrease
                insertion_buffer.insert(insertion_buffer.end(),deletion_buffer_probables.begin(),deletion_buffer_probables.end());
            }

            return min_element;
        }

        void push(int level_index,vector<T>& ins_elements){
            if(level_index == 0)total_elements = total_elements + ins_elements.size();

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
                //I do not see any issue of keeping this IF STATEMENT completely outside the while loop after it . As it is pretty much not possible to increase the upbuffer to more than twice its capacity .
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

        void pull(int level_index,vector<T>& ins_elements){ //Note that the vector given should be empty
            if(total_elements == 0)return; //Just optimisation when there are no elements in any element of priority queue , just dont even try to pull
            if(level_index >= NUM_LEVELS)return; //HARD STOP HARD STOP HARD STOP 

            //1)Define how many elements we should pull based on the level_index
            int num_pull;
            int pulled = 0;
            if(level_index == 0) num_pull = ins_buf_cap;
            else{
                num_pull = levels[level_index - 1].Level_num;
            }

            //2)Add first downbuffer's elements to the ins_elements vector. Either as many as needed or as many as possible .
            DownBuffer<T>* down1 = (levels[level_index].head) -> next;
            if(down1 != levels[level_index].tail){
                sort((down1->elements).begin(),(down1->elements).end(),greater<T>());
                while(pulled < num_pull && !(down1 -> elements).empty()){
                    pulled++;
                    ins_elements.push_back((down1->elements).back());
                    (down1 -> elements).pop_back();
                }
                down1 -> recalc_pivot();
                //If the downbuffer became empty just delete it and move down1 forward once
                if((down1 ->elements).size() == 0){
                    (down1 -> prev) -> next = down1 -> next;
                    (down1 -> next) -> prev = down1 -> prev;
                    DownBuffer<T>* todel = down1;
                    down1 = down1 -> next;
                    delete todel;
                    levels[level_index].cur_down_buffers--;
                }
            }

            //3)Check if ins_elements has enough elements .
            //If it does then we must return 
            if(pulled >= num_pull) return;

            //4)If the function does not return above it means that we do not have enough elements in ins_elements yet
            //Try adding some elements from the down1 , down1 was moved forward
            if(down1 != levels[level_index].tail){
                sort((down1->elements).begin(),(down1->elements).end(),greater<T>());
                while(pulled < num_pull && !(down1->elements).empty()){
                    pulled++;
                    ins_elements.push_back((down1->elements).back());
                    (down1->elements).pop_back();
                }
                down1 -> recalc_pivot();
                //We are able to return here due to the size of second downbuffer being atleast num_pull if the second downbuffer exists
                return;
            }else{
                //5)If the control reaches here it implies that the current level does not contain any more down buffers
                UpBuffer<T>& up = levels[level_index].Up;
                int initial_up_size = up.elements.size();
                pull(level_index + 1,up.elements);
                //Added Level_num elements to upbuffer ideally after above recursive pull
                //Sort the upbuffer
                sort(up.elements.begin(),up.elements.end(),greater<T>());
                //Now put the required number of elements into down buffers num_pull after taking the required amount of elements into ins_elements . But note that up buffer size should not decrease ? according to paper ? ... check
                while(pulled < num_pull && !up.elements.empty()){
                    pulled++;
                    ins_elements.push_back(up.elements.back());
                    up.elements.pop_back();
                }

                //according to paper we should put enough elements into downbuffers as to make the number of elements into upbuffer equal to initial
                //altho this can't happen WHEN THE PULL OPERATION FROM ABOVE LEVELS ITSELF DOES NOT give many elements (edge cases )

                //How making of downbuffers works ? 
                //Make downbuffers from biggest elements and move backwards 
                //We must do this as the first downbuffer can be O(down_buffer_min) but others must have more than down_buffer_min
                DownBuffer<T>* cur = levels[level_index].tail;
                for(int i = initial_up_size;i < up.elements.size();){
                    DownBuffer<T>* temp = new DownBuffer<T>(levels[level_index].down_buffer_cap);
                    levels[level_index].cur_down_buffers++;
                    (cur -> prev) -> next = temp;
                    temp -> prev = cur -> prev; 
                    cur -> prev = temp;
                    temp -> next = cur;
                    cur = temp;
                    while((cur->elements).size() < levels[level_index].down_buffer_min && i < up.elements.size()){
                        cur -> insert(up.elements[i]);
                        i++;
                    }
                }
                if(up.elements.size() > initial_up_size) up.elements.erase(up.elements.begin() + initial_up_size , up.elements.end());

                //Now simply take remaining elements starting from the first down buffer , sort before taking tho

            }
            
        }
};


