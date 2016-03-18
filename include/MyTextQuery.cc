#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <set>
#include "MyTextQuery.h"
#include "StringUtils.h"

TextQuery::TextQuery(const std::string &enDictFile, const std::string &chDictFile)
    : enDict_(enDictFile), chDict_(chDictFile) 
{
  readFile(0);  // en
  readFile(1);  // ch
  index_.buildIndex(dict_);  // 建立索引
}


void TextQuery::readFile(int flag) {
  std::ifstream in;
  std::string dictFile = (flag == 0 ? enDict_: chDict_);
  if(!stringutils::openRead(in, dictFile))
    std::cerr << "Can't open: " + dictFile << std::endl;
  std::string line;
  while(getline(in, line)) {
    std::istringstream iss(line);
    std::string str;
    int freq;
    iss >> str >> freq;
    Word wd;
    wd.word_ = str;
    wd.distance_ = 20;
    wd.frequency_ = freq;
    dict_.insert(std::make_pair(str, wd));
  }
  in.close();
  in.clear();
}


std::string TextQuery::query(const std::string &input) {
  std::priority_queue<Word> pq;
  // 若在词库中, 直接返回
  if(dict_.find(input) != dict_.end())
    return input;
  // 否则进行计算
  std::set<Word> range_set;
  index_.getRange(input, range_set);
  for(auto iter = range_set.begin(); iter != range_set.end(); ++iter) {
    Word wd = *iter;
    wd.distance_ = stringutils::editDistance(input, iter->word_);
    pq.push(wd);
  }
  std::string res;
  // 优先级队列中的第一个元素即为最匹配的元素
  if(!pq.empty())
    res = pq.top().word_;
  else
    res = "Try again";
  return res;
}
