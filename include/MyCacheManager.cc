#include <iostream>
#include "MyCacheManager.h"


CacheManager::CacheManager(const std::string &cacheFile, size_t cacheNum, int updateFrequence)
    : cache_file_(cacheFile),
      cache_num_(cacheNum),
      isStarted_(true),
      caches_(cacheNum),
      flags_(cacheNum),
      empty_(mutex_),
      full_(mutex_) 
{
  totalCache_.readCacheFile(cache_file_);
  auto iter1 = totalCache_.getCacheRef().begin();
  auto iter2 = totalCache_.getCacheRef().end();
  for(auto &c : caches_)
    c.getCacheRef().insert(iter1, iter2);
  for(std::vector<Cache>::size_type i = 0; i != caches_.size(); ++i) {
    caches_[i].setIndex(i);
    flags_[i] = 0;
    queue_.push(i);
  }
  timer_.setTimer(10, updateFrequence);
  timer_.setTimerCallback(std::bind(&CacheManager::writeToFile, this));
}


CacheManager::~CacheManager() {
  timer_.cancelTimerThread();
  if(isStarted_)
    stop();
}


void CacheManager::start() {
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


Cache *CacheManager::getCachePtr() {
  MutexLockGuard lock(mutex_);
  size_t i;
  while(isStarted_ && queue_.empty())
    full_.wait();
  if(!queue_.empty()) {
    queue_.pop();
    empty_.signal_one();
  }
  return &caches_[i];
}


void CacheManager::giveBackCache(size_t i) {
  MutexLockGuard lock(mutex_);
  queue_.push(i);
  if(queue_.size() == 1)
    full_.signal_one();
}


void CacheManager::writeToFile() {
  for(size_t i = 0; i != cache_num_; ++i) {
    auto begin = caches_[i].getCacheRef().begin();
    auto end = caches_[i].getCacheRef().end();
    totalCache_.getCacheRef().insert(begin, end);
  }
  totalCache_.writeCacheFile(cache_file_);
  updateCache();
}


void CacheManager::updateCache() {
  auto begin = totalCache_.getCacheRef().begin();
  auto end = totalCache_.getCacheRef().end();
  for(size_t i = 0; i != cache_num_;) {
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
