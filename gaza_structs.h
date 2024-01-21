
#include<arpa/inet.h>
#include<sys/socket.h>
#include "gaza_constants.h"

struct LoadStruct
{
    int sock_portal;
    sockaddr_in zaun_addr;
    socklen_t zaun_addr_len;
    uint8_t zaun_load_data[DEFAULT_MTU];
    int zaun_load_len;
};