#include <iostream>
#include "MyCacheManager.h"


CacheManager::CacheManager(const std::string &cacheFile, size_t cacheNum, int updateFrequence)
    : cache_file_(cacheFile),
      cache_sz_(cacheNum),
      isStarted_(false),
      //caches_(cacheNum),
      //flags_(cacheNum),
      empty_(mutex_),
      full_(mutex_) 
{
  timer_.setTimer(10, updateFrequence);
  timer_.setTimerCallback(std::bind(&CacheManager::writeToFile, this));  // 定时回写磁盘
}


CacheManager::~CacheManager() {
  timer_.cancelTimerThread();
  if(isStarted_)
    stop();
}


void CacheManager::start() {
  isStarted_ = true;
  // 读取cache文件内容至全局cache
  global_cache_.readCacheFile(cache_file_);
  // 初始化缓存池（填满缓存池）
  while(caches_.size() <= cache_sz_)
    caches_.push(global_cache_);

  /*
  // 初始化缓存池
  auto iter1 = global_cache_.getCacheRef().begin();
  auto iter2 = global_cache_.getCacheRef().end();
  for(auto &c : caches_)
    c.getCacheRef().insert(iter1, iter2);
  for(std::vector<Cache>::size_type i = 0; i != caches_.size(); ++i) {
    caches_[i].setIndex(i);
    flags_[i] = 0;
    queue_.push(i);
  }
  */

  timer_.startTimerThread();
}


void CacheManager::stop() {
  if(isStarted_ == false) 
    return ;

  {
    MutexLockGuard lock(mutex_);
    isStarted_ = false;
  }

  full_.signal_all();
}


// 取cache用于查询: 相当于消费者消费
Cache CacheManager::getCacheCopy() {
  MutexLockGuard lock(mutex_);
  Cache ca;
  while(isStarted_ && caches_.empty())
    full_.wait();
  if(!caches_.empty()) {
    ca = caches_.front();
    caches_.pop();
    empty_.signal_one();
  }
  return ca;
}


// 归还cache: 相当于生产者生产
void CacheManager::giveBackCache() {
  MutexLockGuard lock(mutex_);
  while(caches_.size() >= cache_sz_)
    empty_.wait();
  caches_.push(global_cache_);  // 生产一个全局cache的副本
  full_.signal_one();
}


void CacheManager::writeToFile() {
  /*
  for(size_t i = 0; i != cache_sz_; ++i) {
    auto begin = caches_[i].getCacheRef().begin();
    auto end = caches_[i].getCacheRef().end();
    global_cache_.getCacheRef().insert(begin, end);
  }
  */
  MutexLockGuard lock(mutex_);
  global_cache_.writeCacheFile(cache_file_);
  //updateCache();
}


/*
void CacheManager::updateCache() {
  auto begin = global_cache_.getCacheRef().begin();
  auto end = global_cache_.getCacheRef().end();
  for(size_t i = 0; i != cache_sz_;) {
    Cache *uc = getCachePtr();
    size_t index = uc->getIndex();
    if(flags_[index] == 0) {
      uc->getCacheRef().insert(begin, end);
      flags_[index] = 1;
    }
    giveBackCache(index);
    if(index == i)
      ++i;
  }
  for(auto i : flags_)
    i = 0;
  std::cout << "update cache once" << std::endl;
}
*/
