#include<iostream>
#include "Zaun.h"
#include "Zion.h"

using namespace std;
int main(int argv, char **argc)
{
    
    char portal_code;
    int start_port;
    int end_port;
    if (argv<=1)
    {
        
        cout<<"\t\t_________________________________________________________________________\n\t\t|\t\t\tWELCOME TO GAZA, PORTAL TO ZAUN OR ZION\t\t|"<<endl;
        cout<<"\t\t|Choose Portal using: 1 or 2: 1:Zaun[client] 2:Zion[server]:\t\t|";
        cout<<"\b\b\b\b\b\b\b\b\b\b\b\b";
        cin>>portal_code;
        cout<<"\t\t_________________________________________________________________________";
        
        
        cout.flush();
        sleep(1);
        //exit(1);
        
    }
    else
    {
        char portal_name[7]; 
        memset(portal_name,0,7);
        int is_client = memcmp(portal_name,"client",7);
        int is_server = memcmp(portal_name,"server",7);

        if(is_client==0)
        {
            portal_code= 1;
        }
        else if(is_server==0)
        {
            portal_code= 2;
        }
        else
        {
                cout<<"\nGAZA ERROR please start using gaza or gaza client or gaza server"<<endl;
                exit(1);
        }
    }

    if(portal_code=='1')
    {
        system("clear");
        system("echo");
        cout<<"\t\t_________________________________________________\n\t\t|\tZAUN, THE UNDERCITY PORT TO ZION\t|";
        cout<<"\n\t\t_________________________________________________";

        char zion_addr[20];
        cout<<endl<<"-->Enter Zion Address: ";
        cin>>zion_addr;
        cout<<endl<<"-->Enter Start Port[ 1- 65535]: ";
        START_PORT:
        cin>>start_port;
        if((start_port>65535)||(start_port<1))
        {
            cout<<endl<<"-->Please enter a valid Port [ 1- 65535]: ";
            goto START_PORT;
        }
        cout<<endl<<"-->Enter End Port[ 1- 65535]: ";
        END_PORT:
        cin>>end_port;
        if((end_port>65535)||(end_port<0))
        {
            cout<<endl<<"-->Please enter a valid Port [ 1- 65535]: ";
            goto END_PORT;
        }
        cout<<endl<<"<--Runners finding safe route to: "<<zion_addr<<" on "<<start_port<<" and "<<end_port<<endl;
        uint16_t ports_list[65535];
        memset(ports_list,0,sizeof(ports_list));
        int ports_list_index = 0;
        
        for(uint16_t x_port=(start_port-1);x_port<end_port;x_port++)
        {
            ports_list[ports_list_index] = (x_port+1);
            cout<<ports_list[ports_list_index]<<" ";
            ports_list_index++;
            
        }
        cout<<endl<<"Total number of ports: "<<ports_list_index<<endl;
        Zaun zaun(zion_addr,sizeof(zion_addr),ports_list,ports_list_index);
    }
    else if(portal_code=='2')
    {
        system("clear");
        system("echo");
        cout<<"\t\t_________________________________________________\n\t\t|\tZION, THE LEGENDARY CITY OF FREEDOM\t|";
        cout<<"\n\t\t_________________________________________________";

        uint16_t zion_main_port;
        cout<<endl<<"-->Enter Main Port: ";
        cin>>zion_main_port;
        cout<<endl<<"<--Receiving on safe port: "<<zion_main_port<<endl;
        Zion zion(zion_main_port);
    }
    else
    {
        cout<<"\nGAZA ERROR please start choose 1 or 2"<<endl;
    }
    

}
