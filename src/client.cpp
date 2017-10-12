#include "Ipv4Addr.h"
#include <iostream>
#include <string>
#include <sys/socket.h>

using namespace std;

int main(int argc, char **argv) {

  string server_ip;
  short port;
  string request;

  // 初始化服务器地址，端口，请求内容
  if (argc < 4) {
    server_ip = string("127.0.0.1");
    port = atoi(argv[1]);
    request = string("*") + string(argv[2]);
  } else {
    server_ip = string(argv[1]);
    port = atoi(argv[2]);
    request = string("*") + string(argv[3]);
  }
  char buffer[1024] = "";
  Ipv4Addr addr(server_ip.c_str(), port);
  auto sock_addr = addr.convert_to_sockaddr();

  cout << server_ip << " " << port << " " << request << endl;

  // 连接服务器
  int s = socket(AF_INET, SOCK_STREAM, 0);
  connect(s, &sock_addr, sizeof(sock_addr));

  // 发送请求
  send(s, (const void *)request.c_str(), request.size(), 0);

  // 接收响应
  recv(s, buffer, 1024, 0);

  // 输出
  cout << buffer << endl;
  return 0;
}
