/**
 * test ip2region searcher program
 *
 * @author    chenxin<chenxin619315@gmail.com>
 * @date    2015-10-30
*/

#include "ipip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/time.h>

using namespace std;

class IpSearcher {
public:
  IpSearcher(char *dbFilePath) { init(dbFilePath); }

  ~IpSearcher() { destroy(); }

  string search(string ip) {
    char result[128];
    find(ip.c_str(), result);
    return string(result);
  }
};
