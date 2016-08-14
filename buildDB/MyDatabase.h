#ifndef _MY_DATABASE_H_
#define _MY_DATABASE_H_

#include <string>
#include <mysql/mysql.h>
#include "MyConfiguration.h"

#define DEFAULT_DISTANCE 20

namespace mywcs
{

class MyInfo {
 public:
  MyInfo(Configuration *config) {
    SERVER_HOST    = config->getServerHost();
    SQL_USER_NAME  = config->getSqlUserName();
    SQL_PASSWORD   = config->getSqlPassword();
    DB_NAME        = config->getDbName();
    TABLE_ONE_NAME = config->getTableOneName();
    TABLE_TWO_NAME = config->getTableTwoName();
  }

  ~MyInfo() {}

 private:
  MyInfo(const MyInfo&) = delete;
  MyInfo &operator=(const MyInfo&) = delete;

 public:
  std::string SERVER_HOST;
  std::string SQL_USER_NAME;
  std::string SQL_PASSWORD;
  std::string DB_NAME;
  std::string TABLE_ONE_NAME;
  std::string TABLE_TWO_NAME;
};


class MyDB : public MyInfo {
 public:
  MyDB(const std::string &enDictFile, const std::string &chDictFile, Configuration *config);
  ~MyDB();

  void connectDB();
  void buildDictionaryOne(int flag);
  void buildDictionaryTwo(int flag);

 private:
  MyDB(const MyDB&) = delete;
  MyDB &operator=(const MyDB&) = delete;

  void buildTableOne(const std::string&, const std::string&, uint32_t, uint32_t);
  void buildTableTwo(const std::string&, uint32_t, const std::string&, uint32_t, uint32_t);


 private:
  const std::string enDict_;
  const std::string chDict_;
  MYSQL *mysql_conn_ptr_;
};

}  // namespace mywcs

#endif  /* _MY_DATABASE_H_*/
