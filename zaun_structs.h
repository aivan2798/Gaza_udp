#ifndef ZAUN_STRUCTS_H_INCLUDED
#define ZAUN_STRUCTS_H_INCLUDED



#endif // ZAUN_STRUCTS_H_INCLUDED
#include<vector>
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

    deque<int> active_sockets; //list of created sockets that have not yet expired
    map<double,int> time_batch_index;//map of duration and batch index...used when finding the Pc to use that gives the least time.

};
