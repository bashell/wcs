#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <set>
#include <cstdlib>
#include "MyTextQuery.h"
#include "MyWord.h"
#include "StringUtils.h"


TextQuery::TextQuery() : mysql_conn_ptr_(nullptr) {
  connectDB();
}


TextQuery::~TextQuery() {
  mysql_close(mysql_conn_ptr_);
}

/**
 * 连接数据库
 */
void TextQuery::connectDB() {
  mysql_conn_ptr_ = mysql_init(nullptr);
  const char *server_host = info.SERVER_HOST.c_str();
  const char *sql_user_name = info.SQL_USER_NAME.c_str();
  const char *sql_password = info.SQL_PASSWORD.c_str();
  const char *db_name = info.DB_NAME.c_str();
  if(!mysql_real_connect(mysql_conn_ptr_, server_host, sql_user_name, sql_password, db_name, 0, NULL, 0)) {
    fprintf(stderr, "Connection failed\n");
    if(mysql_error(mysql_conn_ptr_))
      fprintf(stderr, "Connection error %d: %s\n", mysql_errno(mysql_conn_ptr_), mysql_error(mysql_conn_ptr_));
  }
  if(mysql_set_character_set(mysql_conn_ptr_, "utf8")) {
    printf("New client character set: %s\n", mysql_character_set_name(mysql_conn_ptr_));
  }
  //std::cout << "Connect success" << std::endl;
}

/**
 * 查询数据库
 *
 * @param input: 输入
 * return 查询结果
 */
std::string TextQuery::queryDB(const std::string &input) {
  std::string result("");
  
  if(queryTableOne(info.TABLE_ONE_NAME, input))  // found in TABLE_ONE
    return input;
  if(queryTableTwo(info.TABLE_TWO_NAME, input, result))  // calculated in TABLE_TWO
    return result;

  result = "Sorry, no matching word, try again";
  return result;
}

/**
 * 查Table1
 *
 * @param table_name1: 表1名称
 * @param input: 输入
 * return 在表1中找到返回true, 否则返回false
 */
bool TextQuery::queryTableOne(const std::string &table_name1, const std::string &input) {
  char sql_select[256];
  const char *table_name = table_name1.c_str();
  const char *input_str = input.c_str();
  int res;
  bool isFound = false;
  MYSQL_RES *res_ptr;
  MYSQL_ROW sqlrow;

  sprintf(sql_select, "SELECT word FROM %s WHERE word = '%s';", table_name, input_str);
  res = mysql_query(mysql_conn_ptr_, sql_select);
  if(res != 0) {
    fprintf(stderr, "SELECT error: %s\n", mysql_error(mysql_conn_ptr_));
  } else {
    res_ptr = mysql_use_result(mysql_conn_ptr_);
    if(res_ptr) {
      if((sqlrow = mysql_fetch_row(res_ptr))) 
        isFound = true;
      mysql_free_result(res_ptr);
    }
  }
  return isFound;
}

/**
 * 查Table2
 * 
 * @param table_name2: 表2名称
 * @param input: 输入
 * @param result: 查询结果
 * return 表2能推荐出结果返回true; 否则返回false
 */
bool TextQuery::queryTableTwo(const std::string &table_name2, const std::string &input, std::string &result) {
  std::priority_queue<Word> pq;
  char sql_select[256];
  const char *table_name = table_name2.c_str();
  int res;
  bool isRecommended = false;
  MYSQL_RES *res_ptr;
  MYSQL_ROW sqlrow;
  std::vector<uint32_t> uvec;
  std::set<Word> range_set;

  stringutils::utf8ToUint32(input, uvec);
  for(const auto item: uvec) {
    sprintf(sql_select, "SELECT word, distance, frequency FROM %s WHERE item = '%d';", table_name, item);
    res = mysql_query(mysql_conn_ptr_, sql_select);
    if(res != 0) {
      fprintf(stderr, "SELECT error: %s\n", mysql_error(mysql_conn_ptr_));
    } else {
      res_ptr = mysql_store_result(mysql_conn_ptr_);
      if(res_ptr) {
        while((sqlrow = mysql_fetch_row(res_ptr))) {
          //std::cout << sqlrow[0] << " " << sqlrow[1] << " " << sqlrow[2] << std::endl;
          Word wd(sqlrow[0], ::atoi(sqlrow[1]), ::atoi(sqlrow[2]));
          wd.distance_ = stringutils::editDistance(input, std::string(sqlrow[0]));
          if(wd.distance_ <= PQ_TRIM_SIZE)
            pq.push(wd);
        }
        mysql_free_result(res_ptr);
      }
    }
  }
  //std::cout << pq.size() << std::endl;
  if(!pq.empty()) {
    result = pq.top().word_;
    isRecommended = true;
  }

  return isRecommended;
}

