#include <iostream>
#include <fstream>
#include "MyConfiguration.h"
#include "StringUtils.h"


Configuration *Configuration::pInstance_ = nullptr;
pthread_once_t Configuration::once_ = PTHREAD_ONCE_INIT;


void Configuration::readConf(const std::string &filename) {
  std::ifstream in;
  if(!stringutils::openRead(in, filename))
    std::cerr << "Can't open: " + filename << std::endl;
  std::string line;

  // 读取配置文件
  while(getline(in, line)) {
    std::string type, content;
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
    else if(type == "cacheNum")
      cache_num_ = ::atoi(content.c_str());
    else if(type == "updateFrequence")
      update_frequence_ = ::atoi(content.c_str());
  }
  in.close();
  in.clear();
}
