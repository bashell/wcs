#include <iostream>
#include "MyDatabase.h"

using namespace mywcs;

int main(int argc, char *argv[])
{
  std::string en_file = "../data/en_out.txt";
  std::string ch_file = "../data/ch_out.txt";

  std::cout << "Build Database begins..." << std::endl;

  Configuration *config = Configuration::getInstance();
  config->readConf("../conf/config");

  MyDB db(en_file, ch_file, config);
  db.buildDictionaryOne(0);
  db.buildDictionaryOne(1);
  db.buildDictionaryTwo(0);
  db.buildDictionaryTwo(1);

  std::cout << "Build Database success..." << std::endl;

  return 0;
}
