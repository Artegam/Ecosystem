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
      string language;
      map<string, map<string, string>> dictionaries;
      map<string, string> dictionary;

    public:
      Dictionary ();
      void selectDictionary ();
      void loadDictionaries ();
      string translate (string sentence);

      list<list<string>> getFile (string filename);
      map<string, string> getTheme ();
      list<string> getLanguages ();
      string getLanguage ();

      void setLanguage (string lang);
      void setLanguage (const int index);

  };

}

#endif
