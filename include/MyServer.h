#ifndef _MY_SERVER_H_
#define _MY_SERVER_H_

#include <vector>
#include <string>
#include "TcpServer.h"
#include "ThreadPool.h"
#include "InetAddress.h"
#include "MyConfiguration.h"
#include "MyLog.h"
#include "MyCacheManager.h"
#include "MyTextQuery.h"


class Server {
 public:
  Server(Configuration *config);
  ~Server();

  void start();

  //Log *getLog() { return &log_; }
 private:
  Server(const Server&) = delete;
  Server &operator=(const Server&) = delete;

  void connectCallback(const TcpConnectionPtr &conn);
  void messageCallback(const TcpConnectionPtr &conn);
  void closeCallback(const TcpConnectionPtr &conn);
  void compute(const std::string &word, Cache &cache, const TcpConnectionPtr &conn);
  
 private:
  InetAddress addr_;
  TcpServer server_;
  TextQuery search_;
  CacheManager caches_;
  ThreadPool pool_;
  Log log_;
};

#endif  /* _MY_SERVER_H_ */
