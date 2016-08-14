#include <iostream>
#include "MyServer.h"

using namespace mywcs;

int main()
{
  Configuration *config = Configuration::getInstance();
  config->readConf("../conf/config");
  Server server(config);
  server.start();
  return 0;
}
