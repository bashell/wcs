#ifndef _MY_CACHE_MANAGER_H_
#define _MY_CACHE_MANAGER_H_

#include <string>
#include <vector>
#include <queue>
#include "MutexLock.h"
#include "Condition.h"
#include "TimerThread.h"
#include "MyCache.h"


class CacheManager {
 public:
  CacheManager(const std::string &cacheFile, size_t cacheNum, int updateFrequence);
  ~CacheManager();

  void start();
  void stop();
  Cache getCacheCopy();  // 申请cache
  void giveBackCache();  // 归还cache
  void writeToFile();

  Cache *getGlobalCache() { return &global_cache_; }

 private:
  CacheManager(const CacheManager&) = delete;
  CacheManager &operator=(const CacheManager&) = delete;

 private:
  std::string cache_file_;    // cache文件
  size_t cache_sz_;           // 缓存池大小
  bool isStarted_;            // 缓存池开启标志
  Cache global_cache_;        // 全局cache
  std::queue<Cache> caches_;  // 缓存池
  mutable MutexLock mutex_;
  Condition empty_;
  Condition full_;
  TimerThread timer_;         // 定时器, 定时将全局cache内容回写磁盘文件
};


#endif  /* _MY_CACHE_MANAGER_H_ */
