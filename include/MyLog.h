#ifndef _MY_LOG_H_
#define _MY_LOG_H_

#include <string>
#include <queue>
#include "MutexLock.h"
#include "Condition.h"
#include "Thread.h"


class Log {
 public:
  Log(const std::string &logFile);
  ~Log();

  void start();  
  void stop();   
  void addLog(const std::string &s);

 private:
  Log(const Log&) = delete;
  Log &operator=(const Log&) = delete;
  
  std::string getLog();
  void writeLog(const std::string &s);
  void run();

 private:
  std::string log_file_;
  bool isStarted_;
  std::queue<std::string> queue_;  // 日志队列
  mutable MutexLock mutex_;
  Condition cond_;
  Thread thread_;  // 负责日志的读取与写入
};

#endif  /* _MY_LOG_H_ */
