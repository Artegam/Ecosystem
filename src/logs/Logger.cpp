#include "Logs.h"

using namespace std;
using namespace Logs;


Logger::Logger (string filename) {
  this->filename = filename;
}

void Logger::log (Loggable * l) {
  ofstream file(filename.c_str(), ios::out | ios::app);
  list<string> messages = l->log();
  list<string>::iterator it;

  for(it = messages.begin(); it != messages.end(); it++) {
    file << it->c_str() << endl;
  }

  file.close();
}

string Logger::timestamp () {
  time_t now = time(0);
  tm *ltm = localtime(&now);
  string date = to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);
  string hour = to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);
  return date + "-" + hour + " ";
}

void Logger::log (string message) {
  ofstream file(filename.c_str(), ios::out | ios::app);
  file << timestamp() << message << endl;
  file.close();
}
