#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>

namespace stringutils
{

const int LEN_FOR_TRIM = 512;

// File I/O
std::ifstream &openRead(std::ifstream &is, const std::string &filename);
std::ofstream &openWrite(std::ofstream &os, const std::string &filename);

// Edit Distance
inline int utf8Len(char c);
void utf8ToUint32(const std::string &s, std::vector<uint32_t> &vec);
inline int myMin(int a, int b, int c);
inline int editDistanceHelper(const std::vector<uint32_t> &v1, const std::vector<uint32_t> &v2, int len1, int len2);
int editDistance(const std::string &s1, const std::string &s2);

// trim \r\n
void trimSpace_rn(std::string &word);
// trim \n
void trimSpace_n(std::string &word);

}  // namespace stringutils


#endif  /* _STRING_UTILS_H_ */
