#include <iostream>
#include <sstream>
#include <vector>
#include "MyDatabase.h"
#include "StringUtils.h"


MyDB::MyDB(const std::string &enDictFile, const std::string &chDictFile)
        : enDict_(enDictFile), chDict_(chDictFile), mysql_conn_ptr_(nullptr) {
  connectDB();
}


MyDB::~MyDB() {
  mysql_close(mysql_conn_ptr_);
}


void MyDB::connectDB() {
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
    buildTableOne(info.TABLE_ONE_NAME, word, DEFAULT_DISTANCE, freq);
  }
}


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
      buildTableTwo(info.TABLE_TWO_NAME, item, word, DEFAULT_DISTANCE, freq);
    }
  }
}


void MyDB::buildTableOne(const std::string &table_name1, const std::string &word, uint32_t distance, uint32_t frequency) {
  char sql_insert[256];
  const char *table_name = table_name1.c_str();
  const char *word_ptr = word.c_str();
  int res;
  sprintf(sql_insert, "INSERT INTO %s VALUES('%s', '%d', '%d');", table_name, word_ptr, distance, frequency);
  res = mysql_query(mysql_conn_ptr_, sql_insert);
  if(res != 0) {
    fprintf(stderr, "INSERT error %d: %s\n", mysql_errno(mysql_conn_ptr_), mysql_error(mysql_conn_ptr_));
  }
}


void MyDB::buildTableTwo(const std::string &table_name2, uint32_t item, const std::string &word, uint32_t distance, uint32_t frequency) {
  char sql_insert[256];
  const char *table_name = table_name2.c_str();
  const char *word_ptr = word.c_str();
  int res;
  sprintf(sql_insert, "INSERT INTO %s VALUES('%d', '%s', '%d', '%d');", table_name, item, word_ptr, distance, frequency);
  res = mysql_query(mysql_conn_ptr_, sql_insert);
  if(res != 0) {
    fprintf(stderr, "INSERT error %d: %s\n", mysql_errno(mysql_conn_ptr_), mysql_error(mysql_conn_ptr_));
  }
}

/*
int main()
{
  std::string en = "../data/en_out.txt";
  std::string ch = "../data/ch_out.txt";
  MyDB db(en, ch);
  db.buildDictionaryOne(0);
  db.buildDictionaryOne(1);
  //db.buildDictionaryTwo(0);
  //db.buildDictionaryTwo(1);
  return 0;
}
*/
