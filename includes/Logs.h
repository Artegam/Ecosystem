#ifndef LOGS_H
#define LOGS_H


#include <list>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

namespace Logs {
  // interface
  /// class Loggable - 
  class Loggable {
    private:
      // Operations
    public:
      virtual list<string> log ();
  };

  /// class Logger - 
  class Logger {
    private:
      string filename;
      // Operations
    public:
      Logger (const string filename);
      void log (Loggable * l);
      void log (string message);
  };
}

#endif
