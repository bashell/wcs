#include "MyDatabase.h"

int main(int argc, char *argv[])
{
  std::string en_file = "../data/en_out.txt";
  std::string ch_file = "../data/ch_out.txt";
  MyDB db(en_file, ch_file);
  db.buildDictionaryOne(0);
  db.buildDictionaryOne(1);
  db.buildDictionaryTwo(0);
  db.buildDictionaryTwo(1);

  return 0;
}
