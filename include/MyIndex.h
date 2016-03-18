#ifndef _MY_INDEX_H_
#define _MY_INDEX_H_

#include <string>
#include <set>
#include <unordered_map>
#include "MyWord.h"

class Index {
 public:
  Index();
  ~Index();
  void buildIndex(const std::unordered_map<std::string, Word> &dict);  // 建立索引
  void getRange(const std::string &input, std::set<Word> &range_set);

 private:
  std::unordered_map<uint32_t, std::set<Word>> index_;  // 索引表
};

#endif  /* _MY_INDEX_H_ */
