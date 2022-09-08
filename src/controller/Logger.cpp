#include "Controller.h"

using namespace std;
using namespace Controller;


Logger::Logger (string filename) {
  this->filename = filename;
}

void Logger::log (Loggable l) {
	ofstream file(filename.c_str(), ios::out | ios::app);
  list<string> messages = l.log();
  list<string>::iterator it;

  file << "il y a " << messages.size() << " messages" << endl;

	for(it = messages.begin(); it != messages.end(); it++) {  
		file << it->c_str() << endl;
	}

  file.close();
}


