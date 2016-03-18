#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>

namespace stringutils {
  // File I/O
  std::ifstream &openRead(std::ifstream &is, const std::string &filename);
  std::ofstream &openWrite(std::ofstream &os, const std::string &filename);

  // Edit Distance
  inline int utf8Len(char c);
  inline void utf8ToUint32(const std::string &s, std::vector<uint32_t> &vec);
  int myMin(int a, int b, int c);
  inline int editDistanceHelper(const std::vector<uint32_t> &v1, const std::vector<uint32_t> &v2, int len1, int len2);
  int editDistance(const std::string &s1, const std::string &s2);

  void trimSpace(std::string &word);
}


#endif  /* _STRING_UTILS_H_ */
