#include "Controller.h"

#include <ctime>

using namespace std;
using namespace Controller;


list<string> Loggable::log() {

  list<string> messages;

  time_t dt = time(0);
  tm* n = localtime(&dt);
  string str = asctime(n); //to_string(n->tm_mday) + "-" + to_string(n->tm_mon + 1) + "-" + to_string(n->tm_year + 1900);
  string separator = "=================:__:=========================";
  messages.push_back(str + separator);
  messages.push_back("appel a Loggable::log()");

  return messages;
}


