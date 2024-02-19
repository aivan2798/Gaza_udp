#pragma once

#include<cstring>
#include<iostream>
#include "boxes/safe_deque.h"
#include "zaun_structs.h"
#include "gaza_constants.h"

using namespace std;
class Babylon
{
    SafeDeque<uint8_t *> data_inports; //internal container to use for data to be processed
    SafeDeque<uint8_t *> data_exports; 
    ZaunParams *zaun_params;

    public: Babylon()
    {

    }

    public: void initBabylon(ZaunParams *xzaun_params)
    //SafeDeque<uint8_t *> &xdata_inports,SafeDeque<uint8_t *> &xdata_exports) //Babylon constructor with params: buffer to data received and buffer to put data to send
    {
        
        zaun_params = xzaun_params;
        log("babylon okay");
        //data_inports = &xdata_inports;
        //data_inports = &xdata_exports;
    }

    SafeDeque<uint8_t*>* xstartCT()
    {
        uint8_t *ct_init_head = new uint8_t(5);

        std::memcpy(ct_init_head,&CTRL_CMD,1);
        std::memcpy(&ct_init_head[1],CT_INIT_CMD_HEAD,4);

        data_exports.safe_push_front(ct_init_head);
        return &data_exports;
    }

    SafeDeque<uint8_t*>* startCT()
    {
        //uint8_t *ct_init_head = new uint8_t(4);

        sockaddr_in endpoint_addr = zaun_params->portal_addr; //get server endpoint

        uint8_t *ct_init_head = new uint8_t(5);

        std::memcpy(ct_init_head,&CTRL_CMD,1);
        std::memcpy(&ct_init_head[1],CT_INIT_CMD_HEAD,4);

        START_CT:
        int this_babylon = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP); //create new socket endpoint to use

        int sent_bytes = sendto(this_babylon,ct_init_head,5,0,(sockaddr*)&endpoint_addr,sizeof(endpoint_addr)); //send ct_cmd to endpoint
        


        uint8_t *recv_buff[4];
        memset(recv_buff,0,4);
        sockaddr recv_addr;
        memset(&recv_addr,0,sizeof(recv_addr));
        socklen_t recv_addr_len = sizeof(recv_addr);

        int recved_bytes = recvfrom(this_babylon,recv_buff,4,0,(sockaddr*)&recv_addr,&recv_addr_len);

        if(recved_bytes>0)
        {
            int is_ct_tail = memcmp(CT_INIT_CMD_FIN,recv_buff,4);

            if(is_ct_tail!=0)
            {

                goto START_CT;
            }
        }
        else
        {
            goto START_CT;
        }
        log("start ct finished");
        return &data_exports;
    }

    SafeDeque<uint8_t*>* startCR()
    {
        //uint8_t *ct_init_head = new uint8_t(4);

        sockaddr_in endpoint_addr = zaun_params->portal_addr; //get server endpoint

        START_CR:
        int this_babylon = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP); //create new socket endpoint to use

        int sent_bytes = sendto(this_babylon,CR_INIT_CMD_HEAD,4,0,(sockaddr*)&endpoint_addr,sizeof(endpoint_addr)); //send ct_cmd to endpoint
        


        uint8_t *recv_buff[4];
        memset(recv_buff,0,4);
        sockaddr recv_addr;
        memset(&recv_addr,0,sizeof(recv_addr));
        socklen_t recv_addr_len = sizeof(recv_addr);

        int recved_bytes = recvfrom(this_babylon,recv_buff,4,0,(sockaddr*)&recv_addr,&recv_addr_len);

        if(recved_bytes>0)
        {
            int is_ct_tail = memcmp(CR_INIT_CMD_FIN,recv_buff,4);

            if(is_ct_tail!=0)
            {

                goto START_CR;
            }
        }
        else
        {
            goto START_CR;
        }
        
        return &data_exports;
    }

    void router(uint8_t *data_in, int data_in_size)
    {
        if ((memcmp(data_in,&CTRL_CMD,1))==0)
        {
            log("data is: Controll command");

            cmdRouter(&data_in[1],(data_in_size-1));

        }
        else if ((memcmp(data_in,&DATA_CMD,1))==0)
        {
            log("data is: DATA command");

        }
        else
        {
            log("false data");
        }
    }

    void cmdRouter(uint8_t *data_in, int data_in_size) //router that routes the data sent to it
    {
        
        if ((memcmp(data_in,CT_INIT_CMD_HEAD,4))==0)
        {
            log("data is: CT_INIT_CMD_HEAD");

        }
        else if ((memcmp(data_in,CT_INIT_CMD_FIN,4))==0)
        {
            log("data is: CT_INIT_CMD_FIN");

        }
        else if ((memcmp(data_in,CR_INIT_CMD_HEAD,4))==0)
        {
            log("data is: CR_INIT_CMD_HEAD");

        }
        else if ((memcmp(data_in,CR_INIT_CMD_FIN,4))==0)
        {
            log("data is: CR_INIT_CMD_FIN");

        }
        else if ((memcmp(data_in,CT_INIT_CMD_FIN,4))==0)
        {
            log("data is: CT_INIT_CMD_FIN");

        }
        else
        {
            log("command error");
        }



    }

    void ctrlCT_INIT_CMD_HEAD()
    {
        
    }

    void log(char *logdata)
    {
        cout<<endl<<"\t\t"<<logdata<<endl;
        cout.flush();
    }
};