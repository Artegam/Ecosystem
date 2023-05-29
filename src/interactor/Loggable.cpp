#include "Interactor.h"

#include <ctime>

using namespace std;
using namespace Interactor;


list<string> Loggable::log() {

  list<string> messages;

  time_t dt = time(0);
  tm* n = localtime(&dt);
  string str = asctime(n);
  string separator = "==========================================";
  messages.push_back(str + separator);

  return messages;
}


