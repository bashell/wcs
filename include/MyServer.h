#ifndef _MY_SERVER_H_
#define _MY_SERVER_H_

#include <vector>
#include <string>
#include "MyConfiguration.h"
#include "MyLog.h"
#include "MyCacheManager.h"
#include "MyTextQuery.h"

#include "ThreadPool.h"
#include "InetAddress.h"
#include "TcpServer.h"


class Server {
 public:
  Server(Configuration *config);
  ~Server();

  void start();

 private:
  Server(const Server&) = delete;
  Server &operator=(const Server&) = delete;

  void connectCallback(const TcpConnectionPtr &conn);
  void messageCallback(const TcpConnectionPtr &conn);
  void closeCallback(const TcpConnectionPtr &conn);

  void compute(const std::string &word, LruCache *cache, const TcpConnectionPtr &conn);
  
 private:
  InetAddress addr_;
  TcpServer server_;     // 服务器模块
  TextQuery search_;     // 查询模块
  CacheManager caches_;  // 缓存模块
  ThreadPool pool_;      // 线程池模块
  Log log_;              // 日志模块
  mutable MutexLock cache_mutex_;
};


#endif  /* _MY_SERVER_H_ */
