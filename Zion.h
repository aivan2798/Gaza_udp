#include<iostream>
#include<cstdint>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<cstring>
//#include "gaza_constants.h"

using namespace std;
class Zion
{
    int zion_portal = 0;
    sockaddr_in zion_addr;

    int main_inports_buffer_len = 1500;
    uint8_t *main_inports_buffer;
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

            startListen();

        }

        void startListen()
        {
            sockaddr_in new_zaunist;
            socklen_t new_zaunist_len = sizeof(new_zaunist);
            int recv_bytes = recvfrom(zion_portal,main_inports_buffer,main_inports_buffer_len,0,(sockaddr*)&new_zaunist,&new_zaunist_len);

            routeCaptain(new_zaunist,new_zaunist_len);
        }

        void routeCaptain(sockaddr_in &new_zaunist,socklen_t &new_zaunist_len)
        {
            uint8_t route_id[4];
            memset(route_id,0,sizeof(route_id));

            memcpy(route_id,main_inports_buffer,sizeof(route_id));

            if( (memcmp(route_id,CR_INIT_CMD_HEAD,4)==0))
            {
                cout<<endl<<"CR_INIT_HEAD"<<endl;
                sendto(zion_portal,CR_INIT_CMD_FIN,sizeof(CR_INIT_CMD_FIN),0,(sockaddr*)&new_zaunist,new_zaunist_len);
            }
            
        }


        
    
};
