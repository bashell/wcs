#ifndef _MY_CONFIGURATION_H_
#define _MY_CONFIGURATION_H_

#include <string>
#include <pthread.h>
#include <stdlib.h>  // atexit, atoi


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
  const int getCacheNum() const { return cache_num_; }
  const int getUpdateFrequence() const { return update_frequence_; }

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
  int cache_num_;
  int update_frequence_;
};



#endif  /* _MY_CONFIGURATION_H_ */
