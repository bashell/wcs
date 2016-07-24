#include <iostream>
#include <string.h>
#include "../include/StringUtils.h"


std::ifstream &stringutils::openRead(std::ifstream &is, const std::string &filename) {
  is.close();
  is.clear();
  is.open(filename.c_str());
  if(!is)
    std::cerr << "Can't open: " + filename << std::endl;;
  return is;
}


std::ofstream &stringutils::openWrite(std::ofstream &os, const std::string &filename) {
  os.close();
  os.clear();
  os.open(filename.c_str());
  if(!os)
    std::cerr << "Can't open: " + filename << std::endl;
  return os;
}


/*
 * utf-8编码所占字节数
 * cnt_one = 0 ---> 1
 * cnt_one = 2 ---> 2
 * cnt_one = 3 ---> 3
 * ...
 */
inline int stringutils::utf8Len(char c) {
  int cnt_one = 0;
  while(c & (1 << (7 - cnt_one)))  // 计算从字节最高位开始, 连续为1的位数
    ++cnt_one;
  return cnt_one;
}


/*
 * utf-8 ---> uint32_t
 *
 * 例如,若string s = "严肃", 则s中各成员的unsigned char版本对应的十六进制分别为: 
 * s[0] = e4, s[1] = b8, s[2] = a5, s[3] = e8, s[4] = 82, s[5] = 83
 * '严'的utf-8编码(十六进制)为 e4b8a5
 * '肃'的utf-8编码(十六进制)为 e88283
 */
void stringutils::utf8ToUint32(const std::string &s, std::vector<uint32_t> &vec) {
  for(size_t i = 0; i != s.size(); ++i) {
    uint32_t u = (unsigned char)s[i];  // e4
    int len = utf8Len(s[i]);

    /* 
     * UTF-8 编码方式
     * 0xxxxxxx
     * 110xxxxx 10xxxxxx
     * 1110xxxx 10xxxxxx 10xxxxxx
     * 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
     */
    
    // len == 0时为英文字母, utf-8编码和ASCII编码相同
    // len >= 2时, 进行拼接
    if(len > 1) {
      --len;
      while(len--)
        u = (u << 8) + (unsigned char)s[++i];
    }
    vec.push_back(u);
  }
}


int stringutils::myMin(int a, int b, int c) {
  int res = a < b ? a : b;
  return (res < c ? res : c);
}


inline int stringutils::editDistanceHelper(const std::vector<uint32_t> &v1, const std::vector<uint32_t> &v2, int len1, int len2) {
  int dp[len1+1][len2+1];
  for(int i = 0; i <= len1; ++i) {
    for(int j = 0; j <= len2; ++j) {
      if(i == 0)
        dp[i][j] = j;
      else if(j == 0)
        dp[i][j] = i;
      else if(v1[i-1] == v2[j-1])
        dp[i][j] = dp[i-1][j-1];
      else
        dp[i][j] = 1 + myMin(dp[i][j-1], dp[i-1][j], dp[i-1][j-1]);
    }
  }
  return dp[len1][len2];
}


int stringutils::editDistance(const std::string &s1, const std::string &s2) {
  std::vector<uint32_t> v1, v2;
  utf8ToUint32(s1, v1);
  utf8ToUint32(s2, v2);
  return editDistanceHelper(v1, v2, v1.size(), v2.size());
}


void stringutils::trimSpace_rn(std::string &word) {
  char tmp[512] = {0};
  ::strcpy(tmp, word.c_str());
  tmp[::strlen(tmp)-2] = '\0';  // 去掉行尾\r\n
  word = static_cast<std::string>(tmp);
}


void stringutils::trimSpace_n(std::string &word) {
  char tmp[512] = {0};
  ::strcpy(tmp, word.c_str());
  tmp[::strlen(tmp)-1] = '\0';  // 去掉行尾\n
  word = static_cast<std::string>(tmp);
}
