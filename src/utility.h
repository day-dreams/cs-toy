#ifndef UTILITY
#define UTILITY

#include "IpSearcher.h"
#include "logging.h"
#include <mutex>
#include <string>
using namespace std;

mutex _lock;
IpSearcher searcher("data/17monipdb.dat");

string generate_html(string ip) {
  _lock.lock();
  auto region = searcher.search(ip);
  logging::log(ip + string(" ") + region, logging::INFO);
  _lock.unlock();
  auto html =
      string("HTTP/1.1 200 OK\n\n<html><meta http-equiv=\"Content-Type\" "
             "content=\"text/html; "
             "charset=utf-8\"/><p> hello,you are at ip.daydreams.space</p>     "
             "             <p> your ip address :") +
      ip + " " + region + string("</p></html>");

  return html;
}

#endif