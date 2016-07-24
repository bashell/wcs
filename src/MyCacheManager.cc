#include <iostream>
#include "MyCacheManager.h"


CacheManager::CacheManager(const std::string &cacheFile, size_t cacheNum, int updateFrequence)
    : cache_file_(cacheFile),
      cache_sz_(cacheNum),
      isStarted_(false),
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
  // 缓存池初始化
  while(caches_.size() < cache_sz_)
    caches_.push(global_cache_);
  // 计时器启动
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


// 申请cache (consumer)
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


// 归还cache (producer)
void CacheManager::giveBackCache() {
  MutexLockGuard lock(mutex_);
  while(caches_.size() >= cache_sz_)
    empty_.wait();
  caches_.push(global_cache_);  // 生产一个全局cache的副本
  full_.signal_one();
}


void CacheManager::writeToFile() {
  MutexLockGuard lock(mutex_);
  global_cache_.writeCacheFile(cache_file_);
}
