#include <iostream>
#include <sstream>
#include "makeDictCh.h"


MakeDictCh::MakeDictCh(const std::string &dict_path, const std::string &hmm_path)
    : segment_(dict_path, hmm_path)
{}

MakeDictCh::~MakeDictCh() {
}

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

void MakeDictCh::writeFile(const std::string &filename) {
  std::ofstream out;
  if(!openFileWrite(out, filename))
    std::cerr << "Can't open: " + filename << std::endl;
  for(const auto &f : words_)
    out << f.first << " " << f.second << std::endl;
  out.close();
  out.clear();
}

std::ifstream &MakeDictCh::openFileRead(std::ifstream &is, const std::string &filename) {
  is.close();
  is.clear();
  is.open(filename.c_str());
  return is;
}

std::ofstream &MakeDictCh::openFileWrite(std::ofstream &os, const std::string &filename) {
  os.close();
  os.clear();
  os.open(filename.c_str());
  return os;
}
