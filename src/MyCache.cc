#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "MyCache.h"
#include "StringUtils.h"


LruCache::LruCache() : maxSize_(DEFAULT_CACHE_SIZE) {}

LruCache::~LruCache() {}

/**
 * 读磁盘cache文件内容至内存cache
 *
 * @param filename: cache文件名
 */
void LruCache::readCacheFile(const std::string &filename) {
  std::ifstream in;
  if(!stringutils::openRead(in, filename))
    std::cerr << "Can't open: " + filename << std::endl;
  std::string line, key, val;
  while(getline(in, line)) {
    std::istringstream iss(line);
    iss >> key >> val;
    Node new_node;
    new_node.key_ = key;
    new_node.val_ = val;
    cache_list_.push_back(new_node);             // 更新cache_list_
    cache_list_map_[key] = --cache_list_.end();  // 更新cache_list_map_
  }
  in.close();
  in.clear();
}

/**
 * 将内存cache内容回写磁盘cache文件
 *
 * @param filename: cache文件名
 */
void LruCache::writeCacheFile(const std::string &filename) {
  std::ofstream out;
  if(!stringutils::openWrite(out, filename))
    std::cerr << "Can't open: " + filename;
  for(const auto &item : cache_list_)
    out << item.key_ << " " << item.val_ << std::endl;
  out.close();
  out.clear();
}

/**
 * 判断字符串key是否在cache中
 *
 * @param key: KEY
 * @param val: VAL
 * @return 找到返回true; 未找到返回false
 */
bool LruCache::findIfInCache(const std::string &key, std::string &val) {
  auto iter = cache_list_map_.find(key);
  if(iter == cache_list_map_.end()) {  // 未命中
    return false;
  } else {  // 命中
    val = iter->second->val_;
    return true;
  }
}

/**
 * 用<key,val>更新cache
 *
 * @param key: KEY
 * @param val: VAL
 */
void LruCache::putIntoCache(const std::string &key, const std::string &val) {
  auto iter = cache_list_map_.find(key);
  // 未命中
  if(iter == cache_list_map_.end()) {  
    if(cache_list_.size() == maxSize_) { // cache is full
      cache_list_map_.erase(cache_list_.back().key_);
      cache_list_.pop_back();  // 删除cache_list_的最后一个Node
    }

    // 在表头添加新Node
    Node new_node;
    new_node.key_ = key;
    new_node.val_ = val;
    cache_list_.push_front(new_node);
    cache_list_map_[key] = cache_list_.begin();
    //std::cout << "aaa" << std::endl;
  } 
  // 命中
  else {  
    auto list_iterator = cache_list_map_[key];
    cache_list_.erase(list_iterator);  // 删除命中的Node
    //std::cout << cache_list_.size() << std::endl;

    // 在表头添加新Node
    Node new_node;
    new_node.key_ = key;
    new_node.val_ = val;
    cache_list_.push_front(new_node);
    cache_list_map_[key] = cache_list_.begin();
  }
}

/**
 * 将Master cache内容复制到Slave cache
 *
 * @param lru_c: cache副本
 */
void LruCache::copyLruCache(const LruCache lru_c) {
  cache_list_.clear();
  cache_list_map_.clear();
  //std::cout << lru_c.cache_list_.size() << std::endl;
  for(auto iter = lru_c.cache_list_.cbegin(); iter != lru_c.cache_list_.cend(); ++iter) {
    cache_list_.push_back(*iter);
    cache_list_map_[iter->key_] = --cache_list_.end();
    //std::cout << iter->key_ << " " << cache_list_map_[iter->key_]->val_ << std::endl;
  }
}

