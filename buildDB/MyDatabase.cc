#include <iostream>
#include <sstream>
#include <vector>
#include "MyDatabase.h"
#include "StringUtils.h"

using namespace mywcs;

MyDB::MyDB(const std::string &enDictFile, const std::string &chDictFile, Configuration *config)
        : MyInfo(config), enDict_(enDictFile), chDict_(chDictFile), mysql_conn_ptr_(NULL) 
{
  connectDB();
}


MyDB::~MyDB() 
{
  mysql_close(mysql_conn_ptr_);
}

/**
 * 数据库连接
 */
void MyDB::connectDB() {
  mysql_conn_ptr_ = mysql_init(NULL);
  const char *server_host = SERVER_HOST.c_str();
  const char *sql_user_name = SQL_USER_NAME.c_str();
  const char *sql_password = SQL_PASSWORD.c_str();
  const char *db_name = DB_NAME.c_str();
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
 * 构建数据库表1
 * 
 * 表1
 * | Field     | Type        | Null | Key |
 * ----------------------------------------
 * | word      | varchar(20) | NO   | PRI |
 * | distance  | int(11)     | NO   |     |
 * | frequency | int(11)     | NO   |     |
 * 
 * @param flag: 0读取英文词典, 1读取中文词典
 */
void MyDB::buildDictionaryOne(int flag) {
  std::ifstream in;
  std::string dictFile = (flag == 0 ? enDict_ : chDict_);
  if(!stringutils::openRead(in, dictFile))
    std::cerr << "Can't open: " + dictFile << std::endl;
  
  std::string line;
  while(getline(in, line)) {
    std::istringstream iss(line);
    std::string word;
    uint32_t freq;
    iss >> word >> freq;
    buildTableOne(TABLE_ONE_NAME, word, DEFAULT_DISTANCE, freq);
  }
}

/**
 * 构建数据库表2
 *
 * 表1
 * | Field     | Type        | Null | Key |
 * ----------------------------------------
 * | item      | int(11)     | NO   | MUL |
 * | word      | varchar(20) | NO   | PRI |
 * | distance  | int(11)     | NO   |     |
 * | frequency | int(11)     | NO   |     |
 *
 * @param flag: 0读取英文词典, 1读取中文词典
 */
void MyDB::buildDictionaryTwo(int flag) {
  std::ifstream in;
  std::string dictFile = (flag == 0 ? enDict_ : chDict_);
  if(!stringutils::openRead(in, dictFile))
    std::cerr << "Can't open: " + dictFile << std::endl;

  std::string line;
  while(getline(in, line)) {
    std::istringstream iss(line);
    std::string word;
    uint32_t freq;
    std::vector<uint32_t> uvec;
    iss >> word >> freq;
    stringutils::utf8ToUint32(word, uvec);
    for(const auto item : uvec) {
      buildTableTwo(TABLE_TWO_NAME, item, word, DEFAULT_DISTANCE, freq);
    }
  }
}

/**
 * 表1构建过程
 *
 * @param table_name1: 表1名称
 * @param word: word域
 * @param distance: distance域
 * @param frequency: frequency域
 */
void MyDB::buildTableOne(const std::string &table_name1, const std::string &word, uint32_t distance, uint32_t frequency) {
  char sql_insert[256];
  const char *table_name = table_name1.c_str();
  const char *word_ptr = word.c_str();

  sprintf(sql_insert, "INSERT INTO %s VALUES('%s', '%d', '%d');", table_name, word_ptr, distance, frequency);
  mysql_query(mysql_conn_ptr_, sql_insert);
  /*
  if(res != 0) {
    fprintf(stderr, "INSERT error %d: %s\n", mysql_errno(mysql_conn_ptr_), mysql_error(mysql_conn_ptr_));
  }
  */
}

/**
 * 表2构建过程
 *
 * @param table_name2: 表2名称
 * @param item: item域
 * @param word: word域
 * @param distance: distance域
 * @param frequency: frequency域
 */
void MyDB::buildTableTwo(const std::string &table_name2, uint32_t item, const std::string &word, uint32_t distance, uint32_t frequency) {
  char sql_insert[256];
  const char *table_name = table_name2.c_str();
  const char *word_ptr = word.c_str();

  sprintf(sql_insert, "INSERT INTO %s VALUES('%d', '%s', '%d', '%d');", table_name, item, word_ptr, distance, frequency);
  mysql_query(mysql_conn_ptr_, sql_insert);
  /*
  if(res != 0) {
    fprintf(stderr, "INSERT error %d: %s\n", mysql_errno(mysql_conn_ptr_), mysql_error(mysql_conn_ptr_));
  }
  */
}

