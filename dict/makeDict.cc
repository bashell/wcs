#include "makeDictEn.h"
#include "makeDictCh.h"

int main()
{
  std::string en_input_file = "../data/en_in.txt";
  std::string en_write_file = "../data/en_out.txt";
  std::string ch_input_file = "../data/ch_in.txt";
  std::string ch_write_file = "../data/ch_out.txt";
  const char* const DICT_PATH = "jieba.dict.utf8";
  const char* const HMM_PATH = "hmm_model.utf8";

  // 建立英文词库
  MakeDictEn dic;
  dic.readFile(en_input_file);
  dic.writeFile(en_write_file);

  // 建立中文词库
  MakeDictCh md(DICT_PATH, HMM_PATH);
  md.readFile(ch_input_file);
  md.writeFile(ch_write_file);

  return 0;
}
