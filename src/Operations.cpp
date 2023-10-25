#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;

template<typename T>
class DownBuffer
{
    private:
    vector<T> elements;
    int num_of_elements;
    int capacity;
    T pivot;
    DownBuffer *next;
    DownBuffer *prev;

    DownBuffer(int cap = 16)
    {
        next = nullptr;
        prev = nullptr;
        elements.resize(0);
        num_of_elements = 0;
        capacity = cap;
        pivot = 0;
    }

    //friend class DLL;
};

template<typename T>
class UpBuffer
{
    private:
    vector<T> elements;
    int num_of_elements;
    int capacity;

    UpBuffer(int cap = 16)
    {
        elements.resize(0);
        num_of_elements = 0;
        capacity = cap;
    }

    template<typename S> friend class Level;
};



template<class T>
class DLL
{
    private:
    T *head;
    T *tail;
    int length;
    int max_length;

    DLL(int m_length = 16)
    {
        head = new T(16);
        tail = new T(16);

        head->next = tail;
        tail->prev = head;

        length = 0;
        max_length = m_length;
    }

    void insert()
    {
        T *toInsert = new T(16);
        toInsert->prev = tail->prev;
        toInsert->next = tail;
        toInsert->prev->next = toInsert;
        tail->prev = toInsert;
    };

    void insertAfter(T *Excess)
    {
        T *toInsert = new T(16);
        toInsert->prev = Excess;
        toInsert->next = Excess->next;
        Excess->next->prev = toInsert;
        Excess->next = toInsert;
    };
    void Delete()
    {
        T *temp = tail->prev;
        tail->prev = temp->prev;
        temp->prev->next = tail;

        delete temp;
    };
};

template<typename T>
class Level
{
    private:
    UpBuffer<T> Up;
    DLL<DownBuffer<T> > Down;
    int Level_num;

    public:
    Level(int L_num = 16)
    {
        Level_num = L_num;
        Up = UpBuffer<T>(Level_num);
        Down = DLL<DownBuffer<T> >(pow(Level_num, 1/3));
    }
};

int main(){
    cout<<"running";
  // Need to write the push Functions in the level.
}
