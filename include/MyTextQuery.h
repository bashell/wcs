#ifndef _MY_TEXT_QUERY_H_
#define _MY_TEXT_QUERY_H_

#include <string>
#include <mysql/mysql.h>
#include "MyInfo.h"

#define PQ_TRIM_SIZE 3


class TextQuery {
 public:
  TextQuery();
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
  Information info;
};


#endif  /* _MY_TEXT_QUERY_H_ */
