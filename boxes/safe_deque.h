#pragma once
#include <deque>
#include <mutex>
#include <atomic>
#include <unistd.h>

using namespace std;

template<class T> class SafeDeque:public deque<T>
{
    //deque<uint8_t*> a_deque;
    mutex a_deque_mutex;
    atomic_int item_counter;
    
    

    public:
    void safe_push_back(T datum)
    {
        lock_guard<mutex> aloc(a_deque_mutex);
        this->push_back(datum);

        int cache_counter = item_counter.load();
        item_counter.store(cache_counter+1);
    }

    public:
    void safe_push_front(T datum)
    {
        lock_guard<mutex> aloc(a_deque_mutex);
        this->push_front(datum);
        int cache_counter = item_counter.load();
        item_counter.store(cache_counter+1);
    }

    public:
    void safe_pop_front(T datum)
    {
        lock_guard<mutex> aloc(a_deque_mutex);
        this->pop_front(datum);
        int cache_counter = item_counter.load();
        item_counter.store(cache_counter-1);
    }

    void safe_pop_back(T datum)
    {
        lock_guard<mutex> aloc(a_deque_mutex);
        this->pop_back(datum);
        int cache_counter = item_counter.load();
        item_counter.store(cache_counter-1);
    }

    public:
    int safe_getSize()
    {
        lock_guard<mutex> aloc(a_deque_mutex);
        return this->size();
    }
};