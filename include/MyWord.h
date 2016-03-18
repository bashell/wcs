#ifndef _MY_WORD_H_
#define _MY_WORD_H_

#include <string>

struct Word {
  std::string word_;  // 单词
  int distance_;      // 编辑距离
  int frequency_;     // 词频

  // 重载<
  bool operator<(const Word &other) const {
    if(distance_ != other.distance_)
      return distance_ > other.distance_;
    else if(frequency_ != other.frequency_)
      return frequency_ < other.frequency_;
    else
      return false;
  }
};

#endif  /* _MY_WORD_H_ */
