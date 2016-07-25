#ifndef _MY_INFO_H_
#define _MY_INFO_H_

#include <string>

typedef struct tag {
  std::string SERVER_HOST = "localhost";
  std::string SQL_USER_NAME = "bashell";
  std::string SQL_PASSWORD = "1234";
  std::string DB_NAME = "firstdb";
  std::string TABLE_ONE_NAME = "dictionary";
  std::string COLS_ONE_NAME = "word, distance, frequency";
  std::string TABLE_TWO_NAME = "dictionary_split";
  std::string COLS_TWO_NAME = "item, word, distance, frequency";
} Information;

#define DEFAULT_DISTANCE 20

#endif  /* _MY_INFO_H_ */
