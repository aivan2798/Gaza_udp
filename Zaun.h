#ifndef ZAUN_H_INCLUDED
#define ZAUN_H_INCLUDED

#endif // ZAUN_H_INCLUDED


#include<sys/socket.h>
#include<arpa/inet.h>
#include<vector>
#include<map>
#include "zaun_structs.h"
#include <chrono>
#include <atomic>
#include <future>
#include <cstring>
#include <unistd.h>
class Zaun
{

    //--for windows: WSADATA ws_data;
    ZaunParams zaun_params;
    deque<int> &active_sockets = zaun_params.active_sockets;
    int open_portal = 0;
    public:
    Zaun(char *zion_addr,int zion_addr_len)
    {
        //--causes program to crash after pushing to stack memset(&zaun_params,0,sizeof(zaun_params));
        memcpy(zaun_params.zion_addr,zion_addr,zion_addr_len);

        cout<<endl<<"\t\tObtaining port to: "<<zaun_params.zion_addr<<"\t--|__"<<endl;
        init();
    }

    void init()
    {
        //memset(&ws_data,0,sizeof(ws_data));
       //for windows: int wsa_startup_result = WSAStartup(MAKEWORD(1,1),&ws_data);



        /*
        for windows
        if(wsa_startup_result!=0)
        {
            cout<<endl<<"\t\tinitializing error: 8-(";
            cout.flush();

            exit(0);
        }
        */

        cout<<endl<<"\t\tinitializing complete: 100%";
        cout.flush();

        //socket initialised
        int open_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

        //check socket creation
        if(open_sock<=0)
        {
            cout<<endl<<"\t\tSocket creation failed"<<endl;
            cout.flush();
            exit(0);
        }

        addSocket(open_sock);
        setActivePortal(open_sock);
        cout<<endl<<"\t\tsocket initializing complete: 100%";
        cout.flush();

        findSt();
    }
    void addSocket(int open_sock)
    {
        active_sockets.push_front(open_sock);

    }

    void setActivePortal(int open_sock)
    {
        open_portal = open_sock;

    }

    //find how long the socket takes to start
    void findSt()
    {
        time_t time_struct;
        auto time_now = time(nullptr);
        float time_span = 0;

        atomic_bool trigger;
        trigger.store(false);
        //cout<<endl<<"\n\t\tInitiating Shadow Path Finder Protocol:-";

        //Sleep(5000);
        auto time_after = time(nullptr);

        //int load_weight = send(open_portal,)
        auto duration = (time_after-time_now);

        //animate("\n\t\tInitiating Shadow Path Finder Protocol: ",500,trigger,'-');
        char message[] = "\n\t\tInitiating Shadow Path Finder Protocol: ";
        auto animation = startAnimation(message,500,trigger,'-');

        //while(duration<5)
        //{
            sleep(5);
            time_after = time(nullptr);
            duration = (time_after-time_now);

            //Sleep(500);
            //cout<<"-";
        //}
        trigger.store(true);
        time_span = duration;
        cout<<endl<<"\t\tPath Finder Complete with: "<<time_span<<endl;
        //cout<<endl<<time_now<<endl<<"\t duration: "<<duration;
    }

    future<void> startAnimation(char *msg,int speed,atomic_bool &trigger,char icon)
    {
        return async(Zaun::animate,msg,speed,ref(trigger),icon);
    }

    static void animate(char *msg,int speed,atomic_bool &trigger,char icon)
    {
        cout<<msg;
        cout.flush();
        bool stop;
        do
        {
            cout<<icon;
            cout.flush();
            usleep((speed*1000));
            stop = trigger.load();
        }
        while(stop==false);
    }
};
