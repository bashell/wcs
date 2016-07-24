#include <iostream>
#include <sstream>
#include "makeDictEn.h"

MakeDictEn::MakeDictEn() {}

MakeDictEn::~MakeDictEn() {}

void MakeDictEn::readFile(const std::string &filename) {
  std::ifstream in;
  std::string line;
  if(!openFileRead(in, filename))
    std::cerr << "Can't open: " + filename << std::endl;
  while(getline(in, line)) {
    for(size_t i = 0; i != line.size(); ++i) {
      if(ispunct(line[i]) || isdigit(line[i]))  // 数字和标点符号转换成空格
        line[i] = ' ';
      else if(isupper(line[i]))  // 大写变小写
        tolower(line[i]);
    }
    std::istringstream iss(line);
    std::string word;
    while(iss >> word)  // 构建词典
      ++words_[word];
  }
  in.close();
  in.clear();
}

void MakeDictEn::writeFile(const std::string &filename) {
  std::ofstream out;
  if(!openFileWrite(out, filename))
    std::cerr << "Can't open: " + filename << std::endl;
  for(const auto &wd : words_)
    out << wd.first << " " << wd.second << std::endl;
  out.close();
  out.clear();
}

std::ifstream &MakeDictEn::openFileRead(std::ifstream &is, const std::string &filename) {
  is.close();
  is.clear();
  is.open(filename.c_str());
  return is;
}

std::ofstream &MakeDictEn::openFileWrite(std::ofstream &os, const std::string &filename) {
  os.close();
  os.clear();
  os.open(filename.c_str());
  return os;
}
