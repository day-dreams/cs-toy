#ifndef UTILITY
#define UTILITY

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
using namespace std;

string get_time_now() {
  auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
  stringstream buffer;
  buffer << put_time(localtime(&now), "%F %T");
  return buffer.str();
}

string generate_html(string ip) {
  auto html = string("HTTP/1.1 200 OK\n\n<html><p> hello,you are at ") +
              string("ip.daydreams.space</p><p> your ip address : ") + ip +
              string("</p></html>");
  // cout << html << endl;
  return html;
}

class Logger {
private:
  static fstream logfile;
  static bool need_print;

public:
  Logger() {
    logfile = fstream("log.txt", fstream::app);
    need_print = true;
  }
  void set_no_print() { need_print = false; }
};
#endif