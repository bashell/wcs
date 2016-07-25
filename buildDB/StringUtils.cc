#include <iostream>
#include <string.h>
#include "StringUtils.h"

/**
 * 打开文件并返回输入流
 *
 * @param is: 输入流
 * @param filename: 文件名
 */
std::ifstream &stringutils::openRead(std::ifstream &is, const std::string &filename) {
  is.close();
  is.clear();
  is.open(filename.c_str());
  if(!is)
    std::cerr << "Can't open: " + filename << std::endl;;
  return is;
}

/**
 * 打开文件并返回输出流
 *
 * @param os: 输出流
 * @param filename: 文件名
 */
std::ofstream &stringutils::openWrite(std::ofstream &os, const std::string &filename) {
  os.close();
  os.clear();
  os.open(filename.c_str());
  if(!os)
    std::cerr << "Can't open: " + filename << std::endl;
  return os;
}


/**
 * 计算utf-8编码所占字节数
 *
 * cnt_one = 0 ---> 1
 * cnt_one = 2 ---> 2
 * cnt_one = 3 ---> 3
 * ...
 *
 * @param c: 字符
 * return 字符c的utf8编码所占字节数
 */
inline int stringutils::utf8Len(char c) {
  int cnt_one = 0;
  while(c & (1 << (7 - cnt_one)))  // 计算从字节最高位开始, 连续为1的位数
    ++cnt_one;
  return cnt_one;
}


/**
 * 将utf-8编码的string转换成uint32_t数组
 *
 * 例如,若string s = "严肃", 则s中各成员的unsigned char版本对应的十六进制分别为: 
 * s[0] = e4, s[1] = b8, s[2] = a5, s[3] = e8, s[4] = 82, s[5] = 83
 * '严'的utf-8编码(十六进制)为 e4b8a5
 * '肃'的utf-8编码(十六进制)为 e88283
 *
 * @param s: 待转换字符串
 * @param vec: uint32_t结果数组
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

/**
 * 求三个整数的最小值
 *
 * @param a: int
 * @param b: int
 * @param c: int
 */
int stringutils::myMin(int a, int b, int c) {
  int res = a < b ? a : b;
  return (res < c ? res : c);
}

/**
 * 编辑距离计算辅助函数
 *
 * @param v1: 数组v1
 * @param v2: 数组v2
 * @param len1: v1长度
 * @param len2: v2长度
 * return 两个整型数组的编辑距离
 */
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

/**
 * 计算编辑距离
 *
 * @param s1: 字符串s1
 * @param s2: 字符串s2
 * return 两个字符串的编辑距离
 */
int stringutils::editDistance(const std::string &s1, const std::string &s2) {
  std::vector<uint32_t> v1, v2;
  utf8ToUint32(s1, v1);
  utf8ToUint32(s2, v2);
  return editDistanceHelper(v1, v2, v1.size(), v2.size());
}

/**
 * 截去字符串末尾的'\r\n'
 *
 * @param word: 输入字符串
 */
void stringutils::trimSpace_rn(std::string &word) {
  char tmp[512] = {0};
  ::strcpy(tmp, word.c_str());
  tmp[::strlen(tmp)-2] = '\0';  // 去掉行尾\r\n
  word = static_cast<std::string>(tmp);
}

/**
 * 截去字符串末尾的'\n'
 *
 * @param word: 输入字符串
 */
void stringutils::trimSpace_n(std::string &word) {
  char tmp[512] = {0};
  ::strcpy(tmp, word.c_str());
  tmp[::strlen(tmp)-1] = '\0';  // 去掉行尾\n
  word = static_cast<std::string>(tmp);
}
