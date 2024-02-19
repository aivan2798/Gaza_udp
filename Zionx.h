#pragma once

#include<iostream>
#include<cstdint>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<cstring>
#include<future>
#include<deque>
#include <atomic>
#include <map>
//#include <coroutine>

#include "gaza_structs.h"
#include "boxes/safe_deque.h"
#include "Babylon.h"

class Zionx
{
    int zion_portal = 0;

    SafeDeque<LoadStruct*> recved_structs; //safe deque to the received data in the LoadStruct form

    SafeDeque<uint8_t*> data_to_send; //buffer containing data to send;

    map<uint,Babylon*> babylonians; //map of ips and their associated babylon handlers 
    
    public: Zionx(int port)
    {
        startZion(port);
    }

    void startZion(int16_t zion_port) //start zion server
        {
            sockaddr_in zion_addr; //create empty zion_addr
            memset(&zion_addr,0,sizeof(zion_addr)); //zero set the memset addresses
            zion_addr.sin_addr.s_addr = INADDR_ANY; //use any interface
            zion_addr.sin_port = htons(zion_port); //use zion_port
            zion_addr.sin_family = AF_INET; //use ipv4

            zion_portal = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP); //create server sockjet
            main_captain_killer.store(false); //atomic bool to stop the server concurrently
            
            if(zion_portal<=0) //check if socket was created successfully
            {
                cout<<endl<<"SOCKET ERROR"<<endl;
                cout.flush();

                exit(1); //exit program if socket failed to get created
            }

           int bind_status = bind(zion_portal,(sockaddr*)&zion_addr,sizeof(zion_addr)); //bind the socket to the addresses! not applicable in udp

            if(bind_status<0)
            {
                cout<<endl<<"\t\tBINDING ERROR: "<<bind_status<<endl;
                cout.flush();

                exit(1);
            }

            cout<<endl<<"\t\tSTARTING TO LISTEN: "<<zion_port<<endl;
            cout.flush();

            /*port_handler = async(launch::async,mainCaptain,zion_portal,ref(load_structs),ref(load_structs_lock));*/
            startListen(); //start socket listener

        }

        void startListen() //socket listener definition
        {
            sockaddr_in new_zaunist; //temporary structure to store sender address
            socklen_t new_zaunist_len = sizeof(new_zaunist); //temporary structure to store the size of the sender's address
            int main_inports_buffer_len = 1500; //size of the buffer to store recieved data
            
            while(true)
            {
                uint8_t *main_inports_buffer; //pointer to store address of the receive buffer
                main_inports_buffer = new uint8_t[main_inports_buffer_len]; //create receive buffer of length main_inports_buffer_len


                memset(main_inports_buffer,0,main_inports_buffer_len); //zero initialise the receive buffer
                memset(&new_zaunist,0,sizeof(new_zaunist)); //fill the sender addr buffer with zero;
                new_zaunist_len = sizeof(new_zaunist); //set the size of the sender addr buffer
                int recv_bytes = recvfrom(zion_portal,main_inports_buffer,main_inports_buffer_len,0,(sockaddr*)&new_zaunist,&new_zaunist_len); //get sent data and store to buffer
                if(recv_bytes>0) //check if some data was received
                {
                    LoadStruct *tmp_load = new LoadStruct; //create structure to store the received data together with sender addr
                    tmp_load->zaun_addr = new_zaunist;//store sender addr
                    tmp_load->zaun_addr_len = new_zaunist_len; //store sender addr len
                    tmp_load->zaun_load_len = recv_bytes; //store the received data len 
                    //memcpy(tmp_load->zaun_load_data,main_inports_buffer,recv_bytes);
                    tmp_load->zaun_data = main_inports_buffer; //store the address to the received data buffer

                    recved_structs.safe_push_back(tmp_load);

                    int ip_addr = new_zaunist.sin_addr.s_addr;

                    cout<<"received ip: "<<ip_addr<<endl;

                    ZaunParams zaun_params;
                    
                    map<uint,Babylon*>::iterator babylon_iterator =  babylonians.find(ip_addr);

                    if(babylon_iterator!=(babylonians.end()))
                    {
                        (babylon_iterator->second)->router(main_inports_buffer,main_inports_buffer_len);
                    }
                    else
                    {
                        Babylon *babylon = new Babylon;
                        babylon->initBabylon(&zaun_params);
                        (babylon)->router(main_inports_buffer,main_inports_buffer_len);
                        babylonians[ip_addr] = babylon;

                    }
                    
                    
                    
                  //  addLoad(tmp_load);
                }
            }
            //routeCaptain(new_zaunist,new_zaunist_len);
        }
};