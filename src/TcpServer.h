#ifndef TCPSERVER
#define TCPSERVER

#include "Ipv4Addr.h"
#include <arpa/inet.h>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using std::cout;
using std::string;
using std::function;

class TcpServer {
public:
  typedef function<void(int, Ipv4Addr)>
      CallbackType; /* void func(int client_sock,Ipv5Addr client_addr ) */

  TcpServer(string ip, uint16_t port, CallbackType callback)
      : callback(callback) {
    listen_addr = Ipv4Addr(ip.c_str(), port);
  }

  void run() {

    s = socket(AF_INET, SOCK_STREAM, 0);
    int on = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEPORT, (void *)&on, sizeof(on));
    auto server_addr = listen_addr.convert_to_sockaddr();
    bind(s, &server_addr, sizeof(server_addr));
    listen(s, 5);

    sockaddr client_addr;
    socklen_t size = sizeof(client_addr);
    int client_sock;

    cout << "server is running at " << listen_addr.get_ip() << ":"
         << listen_addr.get_port() << '\n';

    while (true) {

      auto client_sock = accept(s, &client_addr, &size);
      Ipv4Addr clientaddr(client_addr);

      callback(client_sock, client_addr);
    }
  }

private:
  int s;
  Ipv4Addr listen_addr;
  CallbackType callback;
};

#endif