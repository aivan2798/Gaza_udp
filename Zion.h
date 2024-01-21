#include<iostream>
#include<cstdint>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<cstring>
#include<future>
#include<deque>
#include "gaza_structs.h"
//#include "gaza_constants.h"

using namespace std;
static atomic_bool main_captain_killer;
class Zion
{
    int zion_portal = 0;
    sockaddr_in zion_addr;

    int main_inports_buffer_len = 1500;
    uint8_t *main_inports_buffer;

    deque<LoadStruct*> load_structs;
    mutex load_structs_lock;
    //this is the connection handler
    future<void> port_handler;
    
    public:
        Zion(int16_t zion_port)
        {
            main_inports_buffer = new uint8_t[main_inports_buffer_len];
            startZion(zion_port);
        }

        void startZion(int16_t zion_port)
        {
            memset(&zion_addr,0,sizeof(zion_addr));
            zion_addr.sin_addr.s_addr = INADDR_ANY;
            zion_addr.sin_port = htons(zion_port);
            zion_addr.sin_family = AF_INET;

            zion_portal = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
            main_captain_killer.store(false);
            
            if(zion_portal<=0)
            {
                cout<<endl<<"SOCKET ERROR"<<endl;
                cout.flush();

                exit(1);
            }

           int bind_status = bind(zion_portal,(sockaddr*)&zion_addr,sizeof(zion_addr));

            if(bind_status<0)
            {
                cout<<endl<<"\t\tBINDING ERROR: "<<bind_status<<endl;
                cout.flush();

                exit(1);
            }

            cout<<endl<<"\t\tSTARTING TO LISTEN: "<<zion_port<<endl;
            cout.flush();

            port_handler = async(launch::async,mainCaptain,zion_portal,ref(load_structs),ref(load_structs_lock));
            startListen();

        }

        void startListen()
        {
            sockaddr_in new_zaunist;
            socklen_t new_zaunist_len = sizeof(new_zaunist);

            while(true)
            {
                memset(main_inports_buffer,0,main_inports_buffer_len);
                memset(&new_zaunist,0,sizeof(new_zaunist));
                new_zaunist_len = sizeof(new_zaunist);
                int recv_bytes = recvfrom(zion_portal,main_inports_buffer,main_inports_buffer_len,0,(sockaddr*)&new_zaunist,&new_zaunist_len);
                if(recv_bytes>0)
                {
                    LoadStruct *tmp_load = new LoadStruct;
                    tmp_load->zaun_addr = new_zaunist;
                    tmp_load->zaun_addr_len = new_zaunist_len;
                    tmp_load->zaun_load_len = recv_bytes;
                    memcpy(tmp_load->zaun_load_data,main_inports_buffer,recv_bytes);

                    addLoad(tmp_load);
                }
            }
            //routeCaptain(new_zaunist,new_zaunist_len);
        }

        void addLoad(LoadStruct *tmp_load)
        {
            load_structs_lock.lock();
            load_structs.push_front(tmp_load);
            load_structs_lock.unlock();
        }

        static void routeCaptain(int this_zion_portal,LoadStruct *tmp_load_structs)
        {
            uint8_t route_id[4];
            sockaddr_in &new_zaunist = tmp_load_structs->zaun_addr;
            socklen_t &new_zaunist_len = tmp_load_structs->zaun_addr_len;
            uint8_t *load = tmp_load_structs->zaun_load_data;
            int load_len = tmp_load_structs->zaun_load_len;
            memset(route_id,0,sizeof(route_id));

            memcpy(route_id,load,sizeof(route_id));

            if( (memcmp(route_id,CR_INIT_CMD_HEAD,4)==0))
            {
                cout<<endl<<"CR_INIT_HEAD"<<endl;
                sendto(this_zion_portal,CR_INIT_CMD_FIN,sizeof(CR_INIT_CMD_FIN),0,(sockaddr*)&new_zaunist,new_zaunist_len);
            }
            else if( (memcmp(route_id,CT_INIT_CMD_HEAD,4)==0))
            {
                cout<<endl<<"CR_INIT_HEAD"<<endl;
                sendto(this_zion_portal,CT_INIT_CMD_FIN,sizeof(CT_INIT_CMD_FIN),0,(sockaddr*)&new_zaunist,new_zaunist_len);
            }
            
        }

        static void mainCaptain(int zion_portalx,deque<LoadStruct*> &main_load,mutex &main_load_lock)
        {
            while((main_captain_killer.load())==false)
            {
                main_load_lock.lock();
                int load_structs_weight = main_load.size();
                main_load_lock.unlock();
                
                if(load_structs_weight>0)
                {
                    main_load_lock.lock();
                    LoadStruct *tmp_load_struct = main_load.back();
                    main_load.pop_back();
                    main_load_lock.unlock();
                    async(launch::async,routeCaptain,zion_portalx,tmp_load_struct);
                }
            }
        }


        
    
};
