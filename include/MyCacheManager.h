#ifndef _MY_CACHE_MANAGER_H_
#define _MY_CACHE_MANAGER_H_

#include <string>
#include <vector>
#include <queue>
#include "MutexLock.h"
#include "Condition.h"
#include "TimerThread.h"
#include "MyCache.h"

namespace mywcs
{

class CacheManager {
 public:
  CacheManager(const std::string &cacheFile, int updateFrequence); 
  ~CacheManager();

  void start();
  void stop();

  void writeToFile();
  void copyMasterToSlave();

  LruCache *getMasterGlobalCache() { return &master_global_cache_; }
  LruCache *getSlaveGlobalCache() { return &slave_global_cache_; }

 private:
  CacheManager(const CacheManager&) = delete;
  CacheManager &operator=(const CacheManager&) = delete;

 private:
  std::string cache_file_;        // cache文件
  bool isStarted_;                // CacheManager开启标志
  LruCache master_global_cache_;  // Master Cache
  LruCache slave_global_cache_;   // Slave  Cache
  mutable MutexLock mutex_;
  TimerThread timer_one_;         // 定时将Slave Cache内容回写磁盘cache文件
  TimerThread timer_two_;         // 定时进行主/从cache复制
};

}  // namespace mywcs

#endif  /* _MY_CACHE_MANAGER_H_ */
