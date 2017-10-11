#ifndef UTILITY
#define UTILITY

#include <string>
using namespace std;

string generate_html(string ip) {
  auto html = string("HTTP/1.1 200 OK\n\n<html><p> hello,you are at ") +
              string("ip.daydreams.space</p><p> your ip address : ") + ip +
              string("</p></html>");
  return html;
}

#endif