/**
 * test ip2region searcher program
 *
 * @author    chenxin<chenxin619315@gmail.com>
 * @date    2015-10-30
*/

#include "ip2region.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/time.h>

using namespace std;

class IpSearcher {
private:
  uint_t (*func_ptr)(ip2region_t, char *, datablock_t);
  ip2region_entry ip2rEntry;

public:
  IpSearcher(char *dbFilePath) {
    // initialize some thing
    datablock_entry datablock;
    char *dbFile = NULL, algorithm[] = "B-tree";
    char line[256];
    double s_time, c_time;
    memset(&datablock, 0x00, sizeof(datablock_entry));

    // 建立数据库
    dbFile = dbFilePath;
    func_ptr = ip2region_btree_search_string;

    // create a new ip2rObj
    ip2region_create(&ip2rEntry, dbFile);
  }

  //
  ~IpSearcher() { ip2region_destroy(&ip2rEntry); }

  string search(string ip) {
    datablock_entry datablock;
    char line[ip.size() + 1];
    for (int i = 0; i != ip.size(); ++i)
      line[i] = ip[i];
    line[ip.size()] = '\0';
    func_ptr(&ip2rEntry, line, &datablock);
    printf("%d|%s in %.5f millseconds\n", datablock.city_id, datablock.region,
           123.0);
    return string(datablock.region);
  }
};
