#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H


#include <map>
#include <list>
#include <string>
#include <iostream>
#include <fstream>


using namespace std;

namespace Translations {

  class Dictionary {
    private:
      list<string> languages;
      unsigned int language = 0;
      map<string, map<string, string>> dictionaries;
      map<string, string> dictionary;

    public:
      Dictionary ();

      list<list<string>> getFile (string filename);
      map<string, string> getTheme ();
      list<string> getLanguages ();
      unsigned int getLanguage ();

      void setLanguage (unsigned int lang = 0);

      void selectDictionary ();
      void loadDictionaries ();
      string translate (string key);
  };

}

#endif
