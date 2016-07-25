#ifndef _MY_DATABASE_H_
#define _MY_DATABASE_H_

#include <string>
#include <mysql/mysql.h>
#include "MyInfo.h"


class MyDB {
 public:
  MyDB(const std::string &enDictFile, const std::string &chDictFile);
  ~MyDB();

  void connectDB();
  void buildDictionaryOne(int flag);
  void buildDictionaryTwo(int flag);

 private:
  void buildTableOne(const std::string&, const std::string&, uint32_t, uint32_t);
  void buildTableTwo(const std::string&, uint32_t, const std::string&, uint32_t, uint32_t);

  MyDB(const MyDB&) = delete;
  MyDB &operator=(const MyDB&) = delete;

 private:
  const std::string enDict_;
  const std::string chDict_;
  MYSQL *mysql_conn_ptr_;
  Information info;
};


#endif  /* _MY_DATABASE_H_*/
