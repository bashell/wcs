# wcs
单词纠错系统（Word Correction System）

简介：字符串的编辑距离可以用来表示字符串之间的相似度。如果一个单词拼写错误，那么可以根据编辑距离计算出与输入单词相似度最高的结果。系统接收用户输入任意单词，然后返回正确的结果供用户参考。

### 系统框架
rptp网络库 + Mysql + Cache
- `rptp网络库`：封装了 ThreadPool 、定时器(Timerfd) 和 I/O mutiplexing。
- `Mysql`：提供数据查询。
- `Cache`：缓存最近的查询结果以减轻数据库的访问压力。
- 整体思路：把TcpServer与ThreadPool结合，用户在回调函数中，把查询任务和数据回发任务封装成函数，交给线程池去处理。TcpServer所在的IO线程主要处理数据收发，线程池内的线程主要处理计算任务：若查询单词在Cache中，则直接取结果返回，否则查Mysql数据库进行处理。

### 模块划分
- **配置模块**
	- 单例模式读取配置文件。
- **预处理模块**
    - 处理中英文语料生成中英文词典文件，形式为：`单词 词频`。
	    - 对中文语料使用“结巴”分词(cppjieba)进行切词。
    - 在Mysql中建立两张表（dictionary、dictionary_split），并将中英文词典文件内容写入两张表中
        - dictionary: word(PRI) distance frequency
        - dictionary_split:  item word(PRI) distance frequency
- **缓存模块**
	- 两个内存Cache：Master Cache用于写操作，Slave Cache用于读操作。它们由CacheManager类统一管理。
	- 线程执行任务时先查询Slave Cache：若在缓存中找到待查询条目，则直接返回查询结果，否则查询数据库。两种情况最后都需将查询结果更新至Master Cache以体现LRU策略。
	- 主从复制 + 定时回写：设立两个定时器线程，第一个定时器线程每隔一段时间会将Master Cache内容复制到Slave Cache中；第二个定时器线程每隔一段时间会Slave Cache中的内容写回磁盘cache文件。定时器触发的时间间隔可由手工指定。
- **查询模块**
	- 若输入单词在缓存Cache中找到，则直接返回。
	- 连接Mysql数据库进行查询：若输入单词在数据库dictionary table中，则直接返回；否则根据输入查询数据库dictionary_split table，得出一个结果集合，将集合中的每个单词同输入单词计算编辑距离，并将符合条件的单词放入优先级队列中，最终向用户返回优先级最高的单词。
- **日志模块**
	- producer-consumer模型。日志存于一个同步队列中，用一个线程专门负责向log文件写日志。
- **工具模块**
	- 封装文件I/O操作，字符串由UTF8编码向uint32\_t数组转换，编辑距离计算 和 string修剪。

### 使用方法
- 安装mysql-server和mysql-client
```bash
$ sudo apt-get update;
$ sudo apt-get install mysql-server;
$ sudo apt-get install libmysqlclient-dev;
```
- 登录Mysql，并在firstdb数据库下创建两个table
```bash
$ mysql> create table dictionary(
      -> word VARCHAR(20) NOT NULL,
      -> distance INT NOT NULL,
      -> frequency INT NOT NULL,
      -> PRIMARY KEY(word)
      -> ) default charset=utf8;
      
$ mysql> create table dictionary_split(
      -> item INT NOT NULL,
      -> word VARCHAR(20) NOT NULL,
      -> distance INT NOT NULL,
      -> frequency INT NOT NULL,
      -> PRIMARY KEY(word)
      -> ) default charset=utf8;
```

- ./wcs_build.sh

- 在bin文件夹运行server
