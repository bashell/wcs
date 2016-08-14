#ifndef _MAKE_DICT_EN_H_
#define _MAKE_DICT_EN_H_

#include <string>
#include <fstream>
#include <unordered_map>

namespace mywcs
{

class MakeDictEn {
 public:
  MakeDictEn();
  ~MakeDictEn();

  void readFile(const std::string &filename);
  void writeFile(const std::string &filename);

 private:
  MakeDictEn(const MakeDictEn&) = delete;
  MakeDictEn &operator=(const MakeDictEn&) = delete;

  std::ifstream &openFileRead(std::ifstream &is, const std::string &filename);
  std::ofstream &openFileWrite(std::ofstream &os, const std::string &filename);

 private:
  std::unordered_map<std::string, size_t> words_;  // 英文词典
};

}  // namespace mywcs

#endif  /* _MAKE_DICT_EN_H_ */
