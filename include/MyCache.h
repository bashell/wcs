#ifndef _MY_CACHE_H_
#define _MY_CACHE_H_

#include <string>
#include <unordered_map>
#include <list>

#define DEFAULT_CACHE_SIZE 1024*1024

namespace mywcs
{

struct Node {
  std::string key_;
  std::string val_;
};

class LruCache {
 public:
  LruCache();
  ~LruCache();

  void readCacheFile(const std::string &filename);
  void writeCacheFile(const std::string &filename);
 
  bool findIfInCache(const std::string &key, std::string &val);
  void putIntoCache(const std::string &key, const std::string &val);

  void copyLruCache(const LruCache &lru_c);

 private:
  std::list<Node> cache_list_;  // cache链表
  std::unordered_map<std::string, std::list<Node>::iterator> cache_list_map_;  // 用于标记key在cache_list_中的位置
  size_t maxSize_;  // cache大小
};

}  // namespace mywcs


#endif  /* _MY_CACHE_H_ */
