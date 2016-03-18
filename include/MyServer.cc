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
      search_(config->getEnDictFile(), config->getChDictFile()),
      // 缓存模块初始化
      caches_(config->getCacheFile(), config->getCacheNum(), config->getUpdateFrequence()),
      // 线程池模块初始化
      pool_(500, 500),
      // 日志模块初始化
      log_(config->getLogFile())
{
  server_.setConnection(std::bind(&Server::connectCallback, this, _1));
  server_.setMessage(std::bind(&Server::messageCallback, this, _1));
  server_.setClose(std::bind(&Server::closeCallback, this, _1));
}


Server::~Server() {
  // 关闭线程池
  pool_.stop();
  // 关闭缓存池
  caches_.stop();
  // 关闭日志
  log_.stop();
}


void Server::start() {
  // 服务器启动
  server_.start();
  // 线程池启动
  pool_.start();
  // 缓存池启动
  caches_.start();
  // 日志启动
  log_.start();
}


void Server::connectCallback(const TcpConnectionPtr &conn) {
  std::ostringstream oss;
  oss << "Client " << conn->getPeerAddr().toIp() << ":"
      << conn->getPeerAddr().toPort() << " is online." << std::endl;
  std::string log_message = oss.str();  // 返回oss中的拷贝
  std::cout << log_message;
  log_.addLog(log_message);
  conn->send("Please input a word.\r\n");
}


void Server::messageCallback(const TcpConnectionPtr &conn) {
  std::string word = conn->receive();
  std::cout << "recv: " << word << std::endl;
  std::cout << "size: " << word.size() << std::endl;
  pool_.addTask(std::bind(&Server::compute, this, word, caches_.getCachePtr(), conn));
}


void Server::closeCallback(const TcpConnectionPtr &conn) {
  std::ostringstream oss;
  oss << "Client " << conn->getPeerAddr().toIp() << ":"
      << conn->getPeerAddr().toPort() << " is offline." << std::endl;
  std::string log_message = oss.str();
  std::cout << log_message;
  log_.addLog(log_message);
}


void Server::compute(const std::string &word, Cache *cache, const TcpConnectionPtr &conn) {
  std::ostringstream oss;
  oss << "Get " << cache->getIndex() << " cache." << std::endl;
  log_.addLog(oss.str());
  oss.str("");
  std::string wd = word;
  stringutils::trimSpace(wd);  // 去掉行尾换行符

  // Cache中存在时直接返回
  auto it = cache->getCacheRef().find(wd);
  if(it != cache->getCacheRef().end()) {
    conn->send(it->second + "\n");
    caches_.giveBackCache(cache->getIndex());
    oss << "Search from cache: " << word << " -> " << it->second << std::endl;
    log_.addLog(oss.str());
    oss.str("");
  } 
  // Cache中没有时从词库中查找
  else {
    std::string result = search_.query(wd);
    // 词库中不存在查找的词时， 插入到Cache中
    if(result != wd) {
      cache->getCacheRef().insert(std::make_pair(wd, result));
      oss << "One pair is add to cache: " << wd << " -> " << result << std::endl;
      log_.addLog(oss.str());
      oss.str("");
    }
    caches_.giveBackCache(cache->getIndex());  // 归还Cache块
    conn->send(result + "\n");
  }
}
