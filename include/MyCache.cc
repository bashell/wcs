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


const std::string Cache::takeFromCache(const std::string &key) {
  std::string res("");
  auto iter = cache_.find(key);
  if(iter != cache_.end()) {
    res = iter->second;
    auto iter_list = std::find(lru_.begin(), lru_.end(), key);
    lru_.erase(iter_list);
    lru_.push_front(key);  // 最近访问元素的放在list的开头位置
  }
  return res;
}


void Cache::putIntoCache(const std::string &key, const std::string &val) {
  cache_.insert(std::make_pair(key, val));
  if(lru_.size() >= maxSize_)
    lru_.pop_back();
  lru_.push_front(key);
}


std::unordered_map<std::string, std::string> &Cache::getCacheRef() {
  return cache_;
}


void Cache::setIndex(size_t index) {
  index_ = index;
}


size_t Cache::getIndex() {
  return index_;
}
