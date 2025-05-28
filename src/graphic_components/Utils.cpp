#include "GraphicComponents.h"

using namespace GraphicComponents;

//find the number of occurences of a character in the string
const unsigned int Utils::occurences(const string str, char c) {
  unsigned int occ = 0;
  unsigned int pos = 0;
  string target = str;

  if(str.length() > 0) {
    pos = target.find('\n');
    while(pos != string::npos && pos < target.length()) {
      occ++;
      target = target.substr(pos, target.length() - pos);
      pos = target.find('\n');
    }
  }
  return occ;
}

