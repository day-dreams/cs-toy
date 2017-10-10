#include "ExpressionParser.h"
#include "Ipv4Addr.h"
#include "TcpServer.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
using namespace std;
using std::string;

string generate_html(string ip) {
  auto html = string("HTTP/1.1 200 OK\n\n<html><p> hello,you are at ") +
              string("ip.daydreams.space</p><p> your ip address : ") + ip +
              string("</p></html>");
  cout << html << endl;
  return html;
}

void callback(int socket, Ipv4Addr addr) {
  int buffersize = 1024;
  char buffer[1024] = "";

  // output connection info
  cout << "connection from:"
       << " (" << addr.get_ip() << ":" << addr.get_port() << ") " << endl;

  recv(socket, buffer, buffersize, 0);

  if (buffer[0] == '*') { // expression parser
    string expression(buffer + 1);
    cout << expression << endl;
    auto result = to_string(parser(expression));
    send(socket, result.c_str(), result.size(), 0);
  } else { // ip.daydreams.space
    auto html = generate_html(addr.get_ip());
    send(socket, html.c_str(), html.size(), 0);
  }
  close(socket);
}

int main(int argc, char **argv) {
  auto port = atoi(argv[1]);
  TcpServer server("0.0.0.0", port, callback);
  server.run();
  return 0;
}