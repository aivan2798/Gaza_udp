#ifndef ZAUN_H_INCLUDED
#define ZAUN_H_INCLUDED

#endif // ZAUN_H_INCLUDED


#include<sys/socket.h>
#include<arpa/inet.h>
#include<vector>
#include<map>
#include "zaun_structs.h"
#include "gaza_constants.h"
#include <chrono>
#include <atomic>
#include <future>
#include <cstring>
#include <unistd.h>
class Zaun
{

    //--for windows: WSADATA ws_data;
    ZaunParams zaun_params;
    deque<int> &active_tx_sockets = zaun_params.active_tx_sockets;
    deque<int> &active_rx_sockets = zaun_params.active_rx_sockets;
    int open_portal = 0;

    int rtimeout = 5 * 1000;
    timeval read_timeout;

    //index to track used and unused sockaddr structures from the active_addr structure
    int active_addr_index = 0;
    public:
    Zaun(char *zion_addr,int zion_addr_len, uint16_t *ports_list,int ports_count)
    {
        memset(&read_timeout,0,sizeof(read_timeout));
        read_timeout.tv_sec = 60;
        read_timeout.tv_usec = 0;

        //--causes program to crash after pushing to stack memset(&zaun_params,0,sizeof(zaun_params));
        memset(zaun_params.zion_addr,0,sizeof(zion_addr));
        memcpy(zaun_params.zion_addr,zion_addr,zion_addr_len);
        zaun_params.active_ports.assign(ports_list,ports_list+(ports_count));

        /*
        Test point: TEST ACTIVE_PORTS VECTOR
        cout<<endl<<"<--Using Ports: ";
        for (int port_now : (zaun_params.active_ports))
        {
            cout<<port_now<<" ";
        }
        */

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


        for (uint16_t port_now : (zaun_params.active_ports))
        {
            //cout<<port_now<<" ";
            sockaddr_in *open_addr = new sockaddr_in;
            memset(open_addr,0,sizeof(*open_addr));

            open_addr->sin_family = AF_INET;
            open_addr->sin_port = htons(port_now);
            open_addr->sin_addr.s_addr = inet_addr(zaun_params.zion_addr);

            zaun_params.active_addrs.push_back(open_addr);

        }

        initCRTunnel();
        initCRTunnel();
        //findSt();
    }
    void addTxSocket(int open_sock)
    {
        active_tx_sockets.push_front(open_sock);

    }

    void addRxSocket(int open_sock)
    {
        active_rx_sockets.push_front(open_sock);

    }

    void setActivePortal(int open_sock)
    {
        open_portal = open_sock;

    }

    //create socket, pick unused sockaddr from active_addr and make a sock active_addr pair
    int makeTxPortal()
    {
        int open_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        //check socket creation
        if(open_sock<=0)
        {
            cout<<endl<<"\t\tSocket creation failed"<<endl;
            cout.flush();
            exit(0);
        }

        addTxSocket(open_sock);
        sockaddr_in *this_sin = zaun_params.active_addrs[active_addr_index];

        zaun_params.socket_portal_map[open_sock] = this_sin;

        return open_sock;
    }

    int makeRxPortal()
    {
        int open_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        //check socket creation
        if(open_sock<=0)
        {
            cout<<endl<<"\t\tSocket creation failed"<<endl;
            cout.flush();
            exit(0);
        }

        addRxSocket(open_sock);
        sockaddr_in *this_sin = zaun_params.active_addrs[active_addr_index];

        zaun_params.socket_portal_map[open_sock] = this_sin;

        return open_sock;
    }

    //find how long the socket takes to start
    void initPortal()
    {
        //sendto(open_portal,)
    }

    bool initCRTunnel()
    {
        int open_sock = makeRxPortal();
       // setActivePortal(open_sock);
        cout<<endl<<"\t\tsocket initializing complete: 100%";
        cout.flush();

        float time_span = 0.0;
        uint8_t raw_datum[5];
        memset(raw_datum,0,(sizeof(raw_datum)));
        cout<<"\t raw datum size: "<<sizeof(raw_datum)<<endl;
        cout.flush();

        setsockopt(open_portal,SOL_SOCKET,SO_RCVTIMEO,&read_timeout,sizeof(read_timeout));
        //setsockopt(open_portal,SOL_SOCKET,SO_RCVTIMEO,(char*)&rtimeout,sizeof(rtimeout));
        float time_after ;
        atomic_bool start_animation;
        start_animation.store(false);
        char *msg = "\n\t\tInitialising receive Connection: ";

        //future<void> load_animation = startAnimation(msg,500,start_animation,'=');

        //cout<<endl<<"using portal: "<<open_portal<<endl;
        //cout.flush();
        //startAnimation(msg,500,start_animation,'=');

        sockaddr recv_addr;
        memset(&recv_addr,0,sizeof(recv_addr));
        socklen_t recv_addr_len = sizeof(recv_addr);

        while(true)
        {
                int xport_weight = sendto(open_portal,CR_INIT_CMD_HEAD,sizeof(CR_INIT_CMD_HEAD),0,(sockaddr*)zaun_params.socket_portal_map[open_portal],sizeof(*zaun_params.socket_portal_map[open_portal]));

                long time_now = time(nullptr);
                cout<<endl<<"\t\tInitial time: "<<time_now<<endl;

                int inport_size = recvfrom(open_portal,raw_datum,sizeof(raw_datum),0,&recv_addr,&recv_addr_len);


                if(inport_size>0)
                {
                    long time_after = time(nullptr);
                    cout<<endl<<"\t\tsignal received at: "<<time_after<<endl;
                    long duration = (time_after-time_now);
                    start_animation.store(true);
                    time_span = duration;
                    cout<<endl<<"\t\tPath Finder Complete with: "<<time_span<<endl;
                    /*cout<<endl<<"dsize: "<<inport_size<<" raw init data: "<<(char*)raw_datum<<endl;*/
                    break;
                }
               // }
       //killman_lock.unlock();

               cout<<"\n\t\tPREMATURE BREAK"<<endl;
               cout.flush();
               break;
        }

        start_animation.store(true);
        //


   return true;

 }

 bool initCTTunnel()
    {
        int open_sock = makeTxPortal();
        setActivePortal(open_sock);
        cout<<endl<<"\t\tsocket initializing complete: 100%";
        cout.flush();

        float time_span = 0.0;
        uint8_t raw_datum[5];
        memset(raw_datum,0,(sizeof(raw_datum)));
        cout<<"\t raw datum size: "<<sizeof(raw_datum)<<endl;
        cout.flush();

        setsockopt(open_portal,SOL_SOCKET,SO_RCVTIMEO,&read_timeout,sizeof(read_timeout));
        //setsockopt(open_portal,SOL_SOCKET,SO_RCVTIMEO,(char*)&rtimeout,sizeof(rtimeout));
        float time_after ;
        atomic_bool start_animation;
        start_animation.store(false);
        char *msg = "\n\t\tInitialising Transmit Connection: ";

        //future<void> load_animation = startAnimation(msg,500,start_animation,'=');

        //cout<<endl<<"using portal: "<<open_portal<<endl;
        //cout.flush();
        //startAnimation(msg,500,start_animation,'=');

        sockaddr recv_addr;
        memset(&recv_addr,0,sizeof(recv_addr));
        socklen_t recv_addr_len = sizeof(recv_addr);

        while(true)
        {
                int xport_weight = sendto(open_portal,CT_INIT_CMD_HEAD,sizeof(CT_INIT_CMD_HEAD),0,(sockaddr*)zaun_params.socket_portal_map[open_portal],sizeof(*zaun_params.socket_portal_map[open_portal]));

                long time_now = time(nullptr);
                cout<<endl<<"\t\tInitial time: "<<time_now<<endl;

                int inport_size = recvfrom(open_portal,raw_datum,sizeof(raw_datum),0,&recv_addr,&recv_addr_len);


                if(inport_size>0)
                {
                    long time_after = time(nullptr);
                    cout<<endl<<"\t\tsignal received at: "<<time_after<<endl;
                    long duration = (time_after-time_now);
                    start_animation.store(true);
                    time_span = duration;
                    cout<<endl<<"\t\tPath Finder Complete with: "<<time_span<<endl;
                    /*cout<<endl<<"dsize: "<<inport_size<<" raw init data: "<<(char*)raw_datum<<endl;*/
                    break;
                }
               // }
       //killman_lock.unlock();

               cout<<"\n\t\tPREMATURE BREAK"<<endl;
               cout.flush();
               break;
        }

        start_animation.store(true);
        //


   return true;

 }

    bool io_time_test()
    {
        float time_span = 0.0;
        uint8_t raw_datum[5];
        memset(raw_datum,0,(sizeof(raw_datum)));
        cout<<"\t raw datum size: "<<sizeof(raw_datum)<<endl;
        cout.flush();

        //setsockopt(open_portal,SOL_SOCKET,SO_RCVTIMEO,&read_timeout,sizeof(read_timeout));
        //setsockopt(open_portal,SOL_SOCKET,SO_RCVTIMEO,(char*)&rtimeout,sizeof(rtimeout));
        float time_after ;
        atomic_bool start_animation;
        start_animation.store(false);
        char *msg = "\n\t\tInitialising Connection: ";

        //future<void> load_animation = startAnimation(msg,500,start_animation,'=');

        //cout<<endl<<"using portal: "<<open_portal<<endl;
        //cout.flush();
        //startAnimation(msg,500,start_animation,'=');

        sockaddr recv_addr;
        memset(&recv_addr,0,sizeof(recv_addr));
        socklen_t recv_addr_len = sizeof(recv_addr);

        while(true)
        {
                int xport_weight = sendto(open_portal,CR_INIT_CMD_HEAD,sizeof(CR_INIT_CMD_HEAD),0,(sockaddr*)zaun_params.socket_portal_map[open_portal],sizeof(*zaun_params.socket_portal_map[open_portal]));

                long time_now = time(nullptr);
                cout<<endl<<"\t\tInitial time: "<<time_now<<endl;

                int inport_size = recvfrom(open_portal,raw_datum,sizeof(raw_datum),0,&recv_addr,&recv_addr_len);


                if(inport_size>0)
                {
                    long time_after = time(nullptr);
                    cout<<endl<<"\t\tafter time: "<<time_after<<endl;
                    long duration = (time_after-time_now);
                    start_animation.store(true);
                    time_span = duration;
                    cout<<endl<<"\t\tPath Finder Complete with: "<<time_span<<endl;
                    /*cout<<endl<<"dsize: "<<inport_size<<" raw init data: "<<(char*)raw_datum<<endl;*/
                    break;
                }
               // }
       //killman_lock.unlock();

               cout<<"\n\t\tPREMATURE BREAK"<<endl;
               cout.flush();
               break;
        }

        start_animation.store(true);
        //


        return true;
        
    }

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
        return async(launch::async,animate,msg,speed,ref(trigger),icon);
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
