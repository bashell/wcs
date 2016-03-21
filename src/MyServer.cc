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
      pool_(1000, 100),
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
  // 关闭缓存池
  caches_.stop();
  // 关闭日志
  log_.stop();
}


void Server::start() {
  // 日志启动
  log_.start();
  // 线程池启动
  pool_.start();
  // 缓存池启动
  caches_.start();
  // 服务器启动
  server_.start();
}


void Server::connectCallback(const TcpConnectionPtr &conn) {
  std::ostringstream oss;
  oss << "Client " << conn->getPeerAddr().toIp() << ":"
      << conn->getPeerAddr().toPort() << " is online." << std::endl;
  std::string log_message = oss.str();  // 返回oss中的拷贝
  std::cout << log_message;
  log_.addLog(log_message);
  conn->send("Please input a word to query:\r\n");
}


void Server::messageCallback(const TcpConnectionPtr &conn) {
  std::string word = conn->receive();
  //std::cout << "recv: " << word << std::endl;
  //std::cout << "size: " << word.size() << std::endl;
  
  // 去掉字符串末尾的\r\n
  if(word.substr(word.size()-2, 2) == "\r\n")
    stringutils::trimSpace_rn(word);
  else if(word.substr(word.size()-1, 1) == "\n")
    stringutils::trimSpace_n(word);
  pool_.addTask(std::bind(&Server::compute, this, word, caches_.getCacheCopy(), conn));
}


void Server::closeCallback(const TcpConnectionPtr &conn) {
  std::ostringstream oss;
  oss << "Client " << conn->getPeerAddr().toIp() << ":"
      << conn->getPeerAddr().toPort() << " is offline." << std::endl;
  std::string log_message = oss.str();
  std::cout << log_message;
  log_.addLog(log_message);
}


void Server::compute(const std::string &word, Cache &cache, const TcpConnectionPtr &conn) {
  std::ostringstream oss;
  oss << "Get a cache for computing." << std::endl;
  log_.addLog(oss.str());
  oss.str("");
  std::string wd = word;

  // cache中存在查询单词, 直接返回
  auto iter = cache.getCacheRef().find(wd);
  if(iter != cache.getCacheRef().end()) {
    conn->send(iter->second + "\r\n");
    oss << "Search from cache: " << wd << " -> " << iter->second << std::endl;
    log_.addLog(oss.str());
    oss.str("");
  } 

  // cache中不存在查询单词, 根据索引查询词库
  else {
    std::string result = search_.query(wd);
    conn->send(result + "\r\n");
    caches_.getGlobalCache()->putIntoCache(wd, result);  // 更新全局cache
    oss << "One pair is add to cache: " << wd << " -> " << result << std::endl;
    log_.addLog(oss.str());
    oss.str("");
  }

  // 计算结束后归还一个cache块
  caches_.giveBackCache();
  oss << "Give back a cache." << std::endl;
  log_.addLog(oss.str());
  oss.str("");
}
