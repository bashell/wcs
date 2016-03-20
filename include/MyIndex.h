#ifndef _MY_INDEX_H_
#define _MY_INDEX_H_

#include <string>
#include <set>
#include <unordered_map>
#include "MyWord.h"


class Index {
 public:
  void buildIndex(const std::unordered_map<std::string, Word> &dict);  // 为词库建立索引
  void getRange(const std::string &input, std::set<Word> &range_set);  // 根据输入单词得出索引范围

 private:
  /*
   * 索引表
   * KEY: 词库中每个单词中的一个字(中文字或者英文字母)对应的uint32_t数值
   * VAL: 词库中所有包括key的单词集合
   */
  std::unordered_map<uint32_t, std::set<Word>> index_;
};


#endif  /* _MY_INDEX_H_ */
