#include "ExpressionParser.h"
#include "Ipv4Addr.h"
#include "LockFreeQueue.h"
#include "TcpServer.h"
#include "logging.h"
#include "utility.h"

#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <vector>
using namespace std;

typedef struct {
  int remote_socket;
  Ipv4Addr remote_addr;
} TcpCon;

// 无锁的(socket,Ipv4Addr)队列
LockFreeQueue<TcpCon> connection_queue;

/*
  server_callback 将每个Tcp连接放入共享队列,每个Tcp连接代表一个客户
*/
void server_callback(int socket, Ipv4Addr addr) {
  TcpCon connection;
  connection.remote_socket = socket;
  connection.remote_addr = addr;
  connection_queue.push(connection);
}

/*
  worker_callback 从共享队列中取出Tcp连接,处理客户的请求
*/
void worker_callback(int i) {

  auto details = string("worker ") + to_string(i) + string(" running.");
  logging::log(details, logging::INFO);

  while (true) {
    auto connection = connection_queue.pop();

    // output connection info
    auto detail = string("worker ") + to_string(i) + string(" connected by") +
                  string(" (") + connection.remote_addr.get_ip() + string(",") +
                  to_string(connection.remote_addr.get_port()) + string(").");
    logging::log(detail, logging::INFO);

    // 准备接受空间
    int buffersize = 1024;
    char buffer[buffersize];
    memset(buffer, '\0', buffersize);

    // 设置超时时限为1,避免被TCP连接攻击
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(connection.remote_socket, SOL_SOCKET, SO_RCVTIMEO,
               (const char *)&timeout, sizeof(timeout));

    // 接收用户请求数据
    auto succeeded = recv(connection.remote_socket, buffer, buffersize, 0);
    if (succeeded == -1) { //接受数据超时,主动关闭,进入下次循环

      auto details = string("客户:(") + connection.remote_addr.get_ip() +
                     string(",") +
                     to_string(connection.remote_addr.get_port()) +
                     string(")") + string("发送请求超时,尝试主动关闭连接");

      logging::log(details, logging::ERROR);
      close(connection.remote_socket);

      continue;
    }

    // 业务逻辑
    if (buffer[0] == '*') { // 算术表达式解析服务

      string expression(buffer + 1);
      auto result = to_string(parser(expression));
      send(connection.remote_socket, result.c_str(), result.size(), 0);

    } else { // ip.daydreams.space,ip查询服务

      auto html = generate_html(connection.remote_addr.get_ip());
      send(connection.remote_socket, html.c_str(), html.size(), 0);
    }

    close(connection.remote_socket);
  }
}

int main(int argc, char **argv) {

  logging::set_print(true);

  // 开启5个worker线程
  vector<thread> workers;
  for (int i = 0; i != 5; ++i) {
    workers.push_back(thread(worker_callback, i));
  }

  // 开启服务器
  auto port = atoi(argv[1]);
  TcpServer server("0.0.0.0", port, server_callback);
  server.run();

  return 0;
}