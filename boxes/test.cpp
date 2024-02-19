#include <iostream>
#include "safe_deque.h"
#include <thread>
#include <future>
#include <coroutine>
#include <chrono>
using namespace std;


static void futureAcess(SafeDeque<uint> &safe_deque, int data)
{
    cout<<endl<<"\t\tnew thread: "<<data<<endl;
    safe_deque.safe_push_back(data);
    sleep(data);
    cout<<endl<<"I have finished: "<<data<<" with len: "<<safe_deque.safe_getSize()<<endl;
    cout.flush();


}

int main()
{
    
    cout<<"welcome to the black parade"<<endl;

    SafeDeque<uint> safe_deque;

    safe_deque.safe_push_front(5);

    cout<<endl<<safe_deque[0]<<endl;
    cout.flush();

    auto finisher = async(launch::deferred,futureAcess,ref(safe_deque),5);
    auto finisher2 = async(launch::deferred,futureAcess,ref(safe_deque), 6);

    cout<<"\n\nwaiting for thread ....."<<endl;
    while(true)
    {
      if((finisher.wait_for(chrono::seconds(0)))==future_status::ready)
      {
        finisher.get();
      }
    }
    //finisher.wait();
    finisher2.get();


}