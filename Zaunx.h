#pragma once

#include<iostream>
#include<cstdint>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<cstring>
#include<future>
#include<deque>
#include <atomic>

#include "Babylon.h"

class Zaunx
{
    sockaddr_in server_addr;
    int server_addr_len;

    Babylon babylon;
    ZaunParams zaun_params;
    public: Zaunx(char *xserver_addr_char,int xserver_addr_len,uint16_t port_no)
    {
        memset(&server_addr,0,sizeof(server_addr));
        server_addr.sin_addr.s_addr = inet_addr(xserver_addr_char);
        server_addr.sin_port = htons(port_no);
        server_addr.sin_family = AF_INET;
        //server_addr = xserver_addr;
        server_addr_len = xserver_addr_len;

        zaun_params.portal_addr=server_addr;
        babylon.initBabylon(&zaun_params);
        babylon.startCT();
        //babylon = 
    
    }


};