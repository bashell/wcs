#include <iostream>
#include <fstream>
#include <functional>  // bind
#include <time.h>

#include "MyLog.h"

using namespace mywcs;


Log::Log(const std::string &logFile)
    : log_file_(logFile),
      isStarted_(false),
      cond_(mutex_),
      thread_(std::bind(&Log::run, this)) 
{}


Log::~Log() {
  if(isStarted_)
    stop();
}


/**
 * 开启日志
 */
void Log::start() {
  isStarted_ = true;
  addLog("Server is start.");
  thread_.start();
}


/**
 * 关闭日志
 */
void Log::stop() {
  isStarted_ = false;
  cond_.signal_all();
  thread_.join();
}


/**
 * 添加日志至队列 (producer)
 *
 * @param s: 日志字符串
 */
void Log::addLog(const std::string &s) {
  MutexLockGuard lock(mutex_);
  queue_.push(addFormatTime(s));
  cond_.signal_one();
}


/**
 * 从队列获取日志 (consumer)
 * 
 * @return 一条日志
 */
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


/**
 * 添加格式化时间
 *
 * @param s: 日志信息
 */
std::string Log::addFormatTime(const std::string &s) {
  char ch[64];
  time_t m_time = time(NULL);
  strftime(ch, sizeof(ch), "%Y-%m-%d|%H-%M-%S : ", localtime(&m_time));
  return std::string(ch) + s;
}

/**
 * 写入日志文件
 *
 * @param s: 日志字符串
 */
void Log::writeLog(const std::string &s) {
  std::ofstream out;
  out.open(log_file_, std::ofstream::app);
  if(!out)
    std::cerr << "Can't open: " + log_file_ << std::endl;
  out << s << std::endl;
  out.close();
  out.clear();
}


/**
 * 日志线程运行函数
 */
void Log::run() {
  while(isStarted_) {
    std::string s = getLog();
    if(s.size() > 0)
      writeLog(s);
  }
  writeLog(addFormatTime("Server is closed."));
}


