#include <vector>
#include <utility>
#include "MyIndex.h"
#include "StringUtils.h"


void Index::buildIndex(const std::unordered_map<std::string, Word> &dict) {
  for(const auto &d : dict) {
    std::string word = d.first;
    std::vector<uint32_t> uvec;
    stringutils::utf8ToUint32(word, uvec);
    for(const auto &u : uvec)
      index_[u].insert(d.second);
  }
}


void Index::getRange(const std::string &input, std::set<Word> &range_set) {
  std::string word = input;
  std::vector<uint32_t> uvec;
  stringutils::utf8ToUint32(word, uvec);
  for(const auto &u : uvec)
    range_set.insert(index_[u].cbegin(), index_[u].cend());
}

