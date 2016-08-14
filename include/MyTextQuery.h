#ifndef _MY_TEXT_QUERY_H_
#define _MY_TEXT_QUERY_H_

#include <string>
#include <mysql/mysql.h>
#include "MyConfiguration.h"

// pruning
#define PQ_TRIM_SIZE 3

#define DEFAULT_DISTANCE 20

namespace mywcs
{

class MyInfo {
 public:
  MyInfo(const std::string &server_host, const std::string &sql_user_name,
         const std::string &sql_password, const std::string &db_name,
         const std::string &table_one_name, const std::string &table_two_name)
      : SERVER_HOST(server_host), SQL_USER_NAME(sql_user_name),
        SQL_PASSWORD(sql_password), DB_NAME(db_name),
        TABLE_ONE_NAME(table_one_name), TABLE_TWO_NAME(table_two_name) {}

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


class TextQuery : public MyInfo {
 public:
  TextQuery(const std::string &server_host, const std::string &sql_user_name,
            const std::string &sql_password, const std::string &db_name,
            const std::string &table_one_name, const std::string &table_two_name);
  ~TextQuery();

  void connectDB();
  std::string queryDB(const std::string &input);

 private:
  bool queryTableOne(const std::string &table_name1, const std::string &input);
  bool queryTableTwo(const std::string &table_name2, const std::string &input, std::string &result);

  TextQuery(const TextQuery&) = delete;
  TextQuery &operator=(const TextQuery&) = delete;

 private:
  MYSQL *mysql_conn_ptr_;
};

}  // namespace mywcs

#endif  /* _MY_TEXT_QUERY_H_ */
