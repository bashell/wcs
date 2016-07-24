#ifndef _MY_WORD_H_
#define _MY_WORD_H_

#include <string>

struct Word {
  Word(const std::string &word, uint32_t distance, uint32_t frequency)
      : word_(word), distance_(distance), frequency_(frequency) {}

  ~Word() {}

  // 重载<
  bool operator<(const Word &other) const {
    if(distance_ != other.distance_)
      return distance_ > other.distance_;
    else if(frequency_ != other.frequency_)
      return frequency_ < other.frequency_;
    else
      return false;
  }
  
  std::string word_;  // 单词
  uint32_t distance_;      // 编辑距离
  uint32_t frequency_;     // 词频
};

#endif  /* _MY_WORD_H_ */
