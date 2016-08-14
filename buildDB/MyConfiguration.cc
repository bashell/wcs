#include <iostream>
#include <fstream>
#include "MyConfiguration.h"
#include "StringUtils.h"

using namespace mywcs;

Configuration *Configuration::pInstance_ = NULL;
pthread_once_t Configuration::once_ = PTHREAD_ONCE_INIT;


/**
 * 配置文件读取
 *
 * @param filename: 配置文件名
 */
void Configuration::readConf(const std::string &filename) {
  std::ifstream in;
  if(!stringutils::openRead(in, filename))
    std::cerr << "Can't open: " + filename << std::endl;

  std::string line;
  while(getline(in, line)) {
    std::string type, content;
    if(line.size() == 0) continue;
    std::string::size_type pos = line.find("=");
    type = line.substr(0, pos);
    content = line.substr(pos + 1);

    if(type == "port")
      port_ = ::atoi(content.c_str());
    else if(type == "logFile")
      log_file_ = content;
    else if(type == "enDict")
      endict_file_ = content;
    else if(type == "chDict")
      chdict_file_ = content;
    else if(type == "cacheFile")
      cache_file_ = content;
    else if(type == "updateFrequence")
      update_frequence_ = ::atoi(content.c_str());
    else if(type == "server_host")
      server_host_ = content;
    else if(type == "sql_user_name")
      sql_user_name_ = content;
    else if(type == "sql_password")
      sql_password_ = content;
    else if(type == "db_name")
      db_name_ = content;
    else if(type == "table_one_name")
      table_one_name_ = content;
    else if(type == "table_two_name")
      table_two_name_ = content;
  }

  in.close();
  in.clear();
}
