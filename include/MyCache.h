#ifndef _MY_CACHE_H_
#define _MY_CACHE_H_

#include <string>
#include <unordered_map>
#include <list>

#define DEFAULT_CACHE_SIZE 20

class Cache {
 public:
  Cache();
  Cache(size_t sz);
  ~Cache();
  void readCacheFile(const std::string &filename);
  void writeCacheFile(const std::string &filename);
  const std::string takeFromCache(const std::string &key);
  void putIntoCache(const std::string &key, const std::string &val);

  std::unordered_map<std::string, std::string> &getCacheRef();
  void setIndex(size_t index);
  size_t getIndex();

 private:
  Cache(const Cache&) = delete;
  Cache &operator=(const Cache&) = delete;

 private:
  std::unordered_map<std::string, std::string> cache_;
  std::list<std::string> lru_;
  size_t index_;
  size_t maxSize_;
};


#endif  /* _MY_CACHE_H_ */
