#include <iostream>
#include <fstream>
#include <functional>  // bind
#include "MyLog.h"


Log::Log(const std::string &logFile)
    : log_file_(logFile),
      isStarted_(false),
      cond_(mutex_),
      thread_(std::bind(&Log::run, this))
{
}


Log::~Log() {
  if(isStarted_)
    stop();
}


void Log::start() {
  isStarted_ = true;
  addLog("Server is start.");
  thread_.start();
}



void Log::stop() {
  isStarted_ = false;
  cond_.signal_all();
  thread_.join();
}


// 添加日志 (producer)
void Log::addLog(const std::string &s) {
  MutexLockGuard lock(mutex_);
  queue_.push(s);
  cond_.signal_one();
}


// 获取日志 (consumer)
std::string Log::getLog() {
  MutexLockGuard lock(mutex_);
  while(queue_.empty() && isStarted_)
    cond_.wait();
  std::string res;
  if(!queue_.empty()) {
    res = queue_.front();
    queue_.pop();
  }
  return res;
}


void Log::writeLog(const std::string &s) {
  std::ofstream out;
  out.open(log_file_, std::ofstream::app);
  if(!out)
    std::cerr << "Can't open: " + log_file_ << std::endl;
  out << s << std::endl;
  out.close();
  out.clear();
}


// 日志线程运行函数
void Log::run() {
  while(isStarted_) {
    std::string s = getLog();
    if(s.size() > 0)
      writeLog(s);
  }
  writeLog("Server is closed.");
}

