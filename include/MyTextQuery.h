#ifndef _MY_TEXT_QUERY_H_
#define _MY_TEXT_QUERY_H_

#include <string>
#include <unordered_map>
#include "MyWord.h"
#include "MyIndex.h"

class TextQuery {
 public:
  TextQuery(const std::string &enDictFile, const std::string &chDictFile);
  std::string query(const std::string &input);

 private:
  TextQuery(const TextQuery&) = delete;
  TextQuery &operator=(const TextQuery&) = delete;

 private:
  /*
   * flag = 0时，读取英文词库
   * flag = 1时，读取中文词库
   */
  void readFile(int flag);
 
 private:
  /*
     struct Word {
       word_;
       distance_;
       frequency_;
       operator<() {...}
     };
  */
  std::unordered_map<std::string, Word> dict_;  //  词库
  const std::string enDict_;
  const std::string chDict_;
  Index index_;
};


#endif  /* _MY_TEXT_QUERY_H_ */
