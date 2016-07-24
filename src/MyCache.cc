#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>  // pair
#include "MyCache.h"
#include "StringUtils.h"


Cache::Cache() : maxSize_(DEFAULT_CACHE_SIZE) {

}


Cache::Cache(size_t sz) : maxSize_(sz) {

}


Cache::~Cache() {

}


void Cache::readCacheFile(const std::string &filename) {
  std::ifstream in;
  if(!stringutils::openRead(in, filename))
    std::cerr << "Can't open: " + filename << std::endl;
  std::string line, key, val;
  while(getline(in, line)) {
    std::istringstream iss(line);
    iss >> key >> val;
    cache_.insert(std::make_pair(key, val));
  }
  in.close();
  in.clear();
}


void Cache::writeCacheFile(const std::string &filename) {
  std::ofstream out;
  if(!stringutils::openWrite(out, filename))
    std::cerr << "Can't open: " + filename;
  for(const auto &c : cache_)
    out << c.first << " " << c.second << std::endl;
  out.close();
  out.clear();
}


void Cache::putIntoCache(const std::string &key, const std::string &val) {
  if(cache_.size() >= maxSize_)  // 缓存满了就清空
    cache_.erase(cache_.begin(), cache_.end());
  cache_.insert(std::make_pair(key, val));
}


std::unordered_map<std::string, std::string> &Cache::getCacheRef() {
  return cache_;
}
