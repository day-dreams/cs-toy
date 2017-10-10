#include"Ipv4Addr.h"
#include<iostream>
#include<sys/socket.h>
#include<string>

using namespace std;

string server_ip="127.0.0.1";

int main(int argc,char**argv){
    auto port = atoi(argv[1]);
    auto request="*"+string(argv[2]);
    char buffer[1024]="";

    Ipv4Addr addr(server_ip.c_str(),port);
    auto sock_addr=addr.convert_to_sockaddr();
    int s=socket(AF_INET,SOCK_STREAM,0);
    connect(s,&sock_addr,sizeof(sock_addr));
    
    send(s,(const void*)request.c_str(),request.size(),0);
    recv(s,buffer,1024,0);
    
    cout<<buffer<<endl;
    return 0;
}

