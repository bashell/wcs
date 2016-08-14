#ifndef _MY_CONFIGURATION_H_
#define _MY_CONFIGURATION_H_

#include <string>
#include <pthread.h>
#include <stdlib.h>  // atexit, atoi

namespace mywcs
{

/*
 * 单例模式读取配置文件(饿汉式)
 */
class Configuration {
 public:
  static Configuration *getInstance() {
    pthread_once(&once_, &initInstance);
    return pInstance_;
  }

  static void initInstance() {
    ::atexit(&destroy);
    pInstance_ = new Configuration;
  }

  static void destroy() {
    delete pInstance_;
  }

  void readConf(const std::string &filename);
  
  const int getPort() const { return port_; }
  const std::string &getLogFile() const { return log_file_; }
  const std::string &getEnDictFile() const { return endict_file_; }
  const std::string &getChDictFile() const { return chdict_file_; }
  const std::string &getCacheFile() const { return cache_file_; }
  const int getUpdateFrequence() const { return update_frequence_; }

  const std::string &getServerHost() const { return server_host_; }
  const std::string &getSqlUserName() const { return sql_user_name_; }
  const std::string &getSqlPassword() const { return sql_password_; }
  const std::string &getDbName() const { return db_name_; }
  const std::string &getTableOneName() const { return table_one_name_; }
  const std::string &getTableTwoName() const { return table_two_name_; }

 private:
  Configuration() {}
  Configuration(const Configuration&) = delete;
  Configuration &operator=(const Configuration&) = delete;

  static Configuration *pInstance_;
  static pthread_once_t once_;

 private:
  int port_;
  std::string log_file_;
  std::string endict_file_;
  std::string chdict_file_;
  std::string cache_file_;
  int update_frequence_;

  std::string server_host_;
  std::string sql_user_name_;
  std::string sql_password_;
  std::string db_name_;
  std::string table_one_name_;
  std::string table_two_name_;
};

}  // namespace mywcs

#endif  /* _MY_CONFIGURATION_H_ */
