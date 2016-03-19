#include <iostream>
#include "MyServer.h"

int main()
{
  Configuration *config = Configuration::getInstance();
  config->readConf("../conf/config.txt");
  Server server(config);
  server.start();
  return 0;
}
