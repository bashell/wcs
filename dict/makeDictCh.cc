#include <iostream>
#include <sstream>
#include "makeDictCh.h"


MakeDictCh::MakeDictCh(const std::string &dict_path, const std::string &hmm_path)
    : segment_(dict_path, hmm_path)
{}

MakeDictCh::~MakeDictCh() {
}

/**
 * 读取中文词库文件并构建词典
 *
 * @param filename: 文件名
 */
void MakeDictCh::readFile(const std::string &filename) {
  std::ifstream in;
  std::string line;
  if(!openFileRead(in, filename))
    std::cerr << "Can't open: " + filename << std::endl;
  while(getline(in, line)) {
    std::vector<std::string> words;
    segment_.Cut(line, words);   // 切词
    for(const auto &wd : words)  // 构建词典
      ++words_[wd];
  }
  in.close();
  in.clear();
}

/**
 * 回写磁盘文件
 *
 * @param filename: 文件名
 */
void MakeDictCh::writeFile(const std::string &filename) {
  std::ofstream out;
  if(!openFileWrite(out, filename))
    std::cerr << "Can't open: " + filename << std::endl;
  for(const auto &f : words_)
    out << f.first << " " << f.second << std::endl;
  out.close();
  out.clear();
}

/**
 * 打开文件并返回输入流
 *
 * @param is: 输入流
 * @param filename: 文件名
 */
std::ifstream &MakeDictCh::openFileRead(std::ifstream &is, const std::string &filename) {
  is.close();
  is.clear();
  is.open(filename.c_str());
  return is;
}

/**
 * 打开文件并返回输出流
 *
 * @param os: 输出流
 * @param filename: 文件名
 */
std::ofstream &MakeDictCh::openFileWrite(std::ofstream &os, const std::string &filename) {
  os.close();
  os.clear();
  os.open(filename.c_str());
  return os;
}
