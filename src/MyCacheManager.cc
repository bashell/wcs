#include <iostream>
#include "MyCacheManager.h"


CacheManager::CacheManager(const std::string &cacheFile, int updateFrequence)
    : cache_file_(cacheFile),
      isStarted_(false) {
  timer_one_.setTimer(10, updateFrequence);
  timer_one_.setTimerCallback(std::bind(&CacheManager::writeToFile, this));
  timer_two_.setTimer(3, updateFrequence/3);
  timer_two_.setTimerCallback(std::bind(&CacheManager::copyMasterToSlave, this));
}

CacheManager::~CacheManager() {
  if(isStarted_)
    stop();
}

/**
 * 开启CacheManager
 */
void CacheManager::start() {
  isStarted_ = true;
  // 读取cache文件内容至Master/Slave Cache
  master_global_cache_.readCacheFile(cache_file_);
  slave_global_cache_.readCacheFile(cache_file_);
  // 计时器启动
  timer_one_.startTimerThread();
  timer_two_.startTimerThread();
}

/**
 * 关闭CacheManager
 */
void CacheManager::stop() {
  if(isStarted_ == false) 
    return ;
  {
    MutexLockGuard lock(mutex_);
    isStarted_ = false;
    timer_one_.cancelTimerThread();
    timer_two_.cancelTimerThread();
  }
}

/**
 * 回写磁盘Cache File
 */
void CacheManager::writeToFile() {
  MutexLockGuard lock(mutex_);
  slave_global_cache_.writeCacheFile(cache_file_);
}

/**
 * 主从Cache复制
 */
void CacheManager::copyMasterToSlave() {
  MutexLockGuard lock(mutex_);
  slave_global_cache_.copyLruCache(master_global_cache_);
}

