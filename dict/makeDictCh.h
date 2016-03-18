#ifndef _MAKE_DICT_CH_H_
#define _MAKE_DICT_CH_H_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include "include/MixSegment.hpp"

class MakeDictCh {
 public:
  MakeDictCh(const std::string &dict_path, const std::string &hmm_path);
  void readFile(const std::string &filename, const std::string &exclude_filename);
  void writeFile(const std::string &filename);

 private:
  std::ifstream &openFileRead(std::ifstream &is, const std::string &filename);
  std::ofstream &openFileWrite(std::ofstream &os, const std::string &filename);
 
 private:
  cppjieba::MixSegment segment_;                   // 用于分词
  std::unordered_map<std::string, size_t> words_;  // 词典
  std::unordered_set<std::string> exclude_;        // 停止词
};

#endif  /* _MAKE_DICT_CH_H_ */
