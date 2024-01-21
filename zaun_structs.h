#ifndef ZAUN_STRUCTS_H_INCLUDED
#define ZAUN_STRUCTS_H_INCLUDED



#endif // ZAUN_STRUCTS_H_INCLUDED
#include<vector>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<deque>
#include<map>

using namespace std;
struct ZaunParams
{
    char zion_addr[24];
    float St = 0;//connection start time
    float Ct = 0;//connection time
    float It = 0;//idle time
    int Pc = 0;//Packet counter...shows the batch item in which to start a new connection
    int Rc = 0;//R count
    int Tc = 0;//T count
    sockaddr_in portal_addr;


    deque<sockaddr_in*> active_addrs;
    deque<int> active_tx_sockets; //list of created send sockets that have not yet expired
    deque<int> active_rx_sockets; //list of created recieve sockets that have not yet expired
    vector<uint16_t> active_ports; //list of zion ports that can be connected to
    map<int,sockaddr_in*> socket_portal_map;
    map<double,int> time_batch_index;//map of duration and batch index...used when finding the Pc to use that gives the least time.

};

struct zionParams
{
    int main_portal;

};