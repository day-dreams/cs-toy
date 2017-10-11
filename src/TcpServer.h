#ifndef TCPSERVER
#define TCPSERVER

#include "Ipv4Addr.h"
#include "logging.h"
#include "utility.h"

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

    // 准备相关变量
    sockaddr client_addr;
    socklen_t size = sizeof(client_addr);
    int client_sock;

    // 创建套接字,设置选项
    s = socket(AF_INET, SOCK_STREAM, 0);
    int on = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEPORT, (void *)&on, sizeof(on));
    auto server_addr = listen_addr.convert_to_sockaddr();

    // 绑定套接字,开始监听
    bind(s, &server_addr, sizeof(server_addr));
    listen(s, 5);

    auto details = string("server is running at ") + listen_addr.get_ip() +
                   string(":") + to_string(listen_addr.get_port());
    logging::log(details, logging::INFO);

    while (true) {

      // 等待一个客户的连接
      auto client_sock = accept(s, &client_addr, &size);
      Ipv4Addr clientaddr(client_addr);

      // 调用约定好的函数来处理连接
      callback(client_sock, client_addr);
    }
  }

private:
  int s;
  Ipv4Addr listen_addr;
  CallbackType callback;
};

#endif