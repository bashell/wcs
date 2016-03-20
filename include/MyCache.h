#ifndef _MY_CACHE_H_
#define _MY_CACHE_H_

#include <string>
#include <unordered_map>
#include <list>

#define DEFAULT_CACHE_SIZE 2048

class Cache {
 public:
  Cache();
  Cache(size_t sz);
  ~Cache();

  void readCacheFile(const std::string &filename);
  void writeCacheFile(const std::string &filename);
 
  void putIntoCache(const std::string &key, const std::string &val);
  std::unordered_map<std::string, std::string> &getCacheRef();

 private:
  std::unordered_map<std::string, std::string> cache_;  // 缓存
  size_t maxSize_;
};


#endif  /* _MY_CACHE_H_ */
