#include<iostream>
#include "Zaun.h"

using namespace std;
int main(int argv, char **argc)
{
    
    char portal_code;
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
        cout<<endl<<"<--Runners finding safe route to: "<<zion_addr<<endl;
        Zaun zaun(zion_addr,sizeof(zion_addr));
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
    }
    else
    {
        cout<<"\nGAZA ERROR please start choose 1 or 2"<<endl;
    }
    

}
