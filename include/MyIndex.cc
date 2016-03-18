#include <vector>
#include <utility>
#include "MyIndex.h"
#include "StringUtils.h"


Index::Index() : index_(26) {

}

Index::~Index() {

}

// 将词库中的单词读入索引表中
void Index::buildIndex(const std::unordered_map<std::string, Word> &dict) {
  for(const auto &iter : dict) {
    std::string word = iter.first;
    std::vector<uint32_t> uvec;
    stringutils::utf8ToUint32(word, uvec);
    for(auto i = 0; i != uvec.size(); ++i)
      //index_.insert(std::make_pair(uvec[i], iter.second));
      index_[uvec[i]].insert(iter.second);
  }
}

// 根据输入单词得出索引范围
void Index::getRange(const std::string &input, std::set<Word> &range_set) {
  std::string word = input;
  std::vector<uint32_t> uvec;
  stringutils::utf8ToUint32(word, uvec);
  for(auto i = 0; i != uvec.size(); ++i)
    range_set.insert(index_[uvec[i]].begin(), index_[uvec[i]].end());
}
