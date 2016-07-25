#include <iostream>
#include <sstream>
#include <string>
#include <functional>  // placesholders
#include "MyServer.h"
#include "StringUtils.h"

using namespace std::placeholders;

Server::Server(Configuration *config)
    : addr_(config->getPort()),
      // 服务器模块初始化 
      server_(addr_),
      // 查询模块初始化
      search_(),
      // 缓存模块初始化
      caches_(config->getCacheFile(), config->getUpdateFrequence()),
      // 线程池模块初始化(池中线程个数一半等于CPU核数)
      pool_(1000, 4),
      // 日志模块初始化
      log_(config->getLogFile()) 
{
  // 回调绑定
  server_.setConnection(std::bind(&Server::connectCallback, this, _1));
  server_.setMessage(std::bind(&Server::messageCallback, this, _1));
  server_.setClose(std::bind(&Server::closeCallback, this, _1));
}

Server::~Server() {
  // 关闭线程池
  pool_.stop();
  // 关闭缓存
  caches_.stop();
  // 关闭日志
  log_.stop();
}

/**
 * 开启服务
 */
void Server::start() {
  // 日志启动
  log_.start();
  // 线程池启动
  pool_.start();
  // 缓存启动
  caches_.start();
  // 服务器启动
  server_.start();
}

/**
 * 连接到来时的回调
 * 
 * @param conn: Tcp连接
 */
void Server::connectCallback(const TcpConnectionPtr &conn) {
  std::ostringstream oss;
  oss << "Client " << conn->getPeerAddr().toIp() << ":"
      << conn->getPeerAddr().toPort() << " is online." << std::endl;
  std::string log_message = oss.str();  // 返回oss中的拷贝
  std::cout << log_message;
  log_.addLog(log_message);
  conn->send("Please input a word to query:\r\n");
}

/**
 * 消息处理的回调
 *
 * @param conn: Tcp连接
 */
void Server::messageCallback(const TcpConnectionPtr &conn) {
  std::string word = conn->receive();
  //std::cout << "recv: " << word << std::endl;
  //std::cout << "size: " << word.size() << std::endl;
  
  // 去掉字符串末尾的\r\n
  if(word.substr(word.size()-2, 2) == "\r\n")
    stringutils::trimSpace_rn(word);
  else if(word.substr(word.size()-1, 1) == "\n")
    stringutils::trimSpace_n(word);
  pool_.addTask(std::bind(&Server::compute, this, word, caches_.getSlaveGlobalCache(), conn));  // 查Slave Cache
}

/**
 * 连接关闭时的回调
 *
 * @param conn: Tcp连接
 */
void Server::closeCallback(const TcpConnectionPtr &conn) {
  std::ostringstream oss;
  oss << "Client " << conn->getPeerAddr().toIp() << ":"
      << conn->getPeerAddr().toPort() << " is offline." << std::endl;
  std::string log_message = oss.str();
  std::cout << log_message;
  log_.addLog(log_message);
}

/**
 * 真正的消息处理过程
 *
 * @param word: 接收到的单词
 * @param cache_ptr: 指向内存cache的指针
 * @param conn: Tcp连接
 */
void Server::compute(const std::string &word, LruCache *cache_ptr, const TcpConnectionPtr &conn) {
  std::ostringstream oss;
  oss << "Query request comes." << std::endl;
  log_.addLog(oss.str());
  oss.str("");

  /*
   * 1. cache命中
   */
  std::string val("");
  if((*cache_ptr).findIfInCache(word, val)) {
    //std::cout << "Cache hit" << std::endl;
    conn->send(val + "\r\n");
    {
      MutexLockGuard lock(cache_mutex_);
      caches_.getMasterGlobalCache()->putIntoCache(word, val);  // 更新Master Cache
    }
    oss << "Search from cache: " << word << " -> " << val << std::endl;
    log_.addLog(oss.str());
    oss.str("");
  }
  
  /*
   * 2. 查数据库
   */
  else {
    //std::cout << "Query DB" << std::endl;
    std::string result = search_.queryDB(word);
    conn->send(result + "\r\n");
    {
      MutexLockGuard lock(cache_mutex_);
      caches_.getMasterGlobalCache()->putIntoCache(word, result);  // 更新Master Cache
    }
    oss << "One pair is add to cache: " << word << " -> " << result << std::endl;
    log_.addLog(oss.str());
    oss.str("");
  }
}

