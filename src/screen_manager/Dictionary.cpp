#include "ScreenManager.h"

using namespace std;
using namespace ScreenManager;

Dictionary::Dictionary () {
  loadDictionaries();
}

list<list<string>> Dictionary::getFile (string filename) {
  ifstream ifs;
  ifs.open (filename.c_str());

  char c = ifs.get();
  string word;
  list<string> line;
  list<list<string>> file;
  while (ifs.good()) {
    if(c == '\n') {
      //EOL
      line.push_back(word);
      file.push_back(line);
      line.clear();
      word = "";
    } else if(c == ';') {
      //separator
      line.push_back(word);
      word = "";
    } else {
      word += c;
    }
    c = ifs.get();
  }

  ifs.close();
  return file;
}

map<string, string> Dictionary::getTheme () {
  map<string, string> theme;
  list<list<string>> file = getFile("theme.csv");

  list<list<string>>::iterator it;
  list<string> line;

  for(it = file.begin(); it != file.end(); it++) {
    line = (*it);
    list<string>::iterator itl;
    itl = line.begin();
    string keyword = (*itl);
    string val = (*++itl);
    theme[keyword] = val;
  }

  return theme;
}

list<string> Dictionary::getLanguages () {
  return languages;
}

unsigned int Dictionary::getLanguage () {
  return this->language;
}

void Dictionary::setLanguage (unsigned int lang) {
  this->language = lang;
  selectDictionary();
}

void Dictionary::selectDictionary() {
  // Selection du dictionnaire et retour
  list<string>::iterator itl = this->languages.begin();
  advance(itl, this->language);

  dictionary = dictionaries[(*itl)];
}

void Dictionary::loadDictionaries () {
  map<string, string> theme = getTheme();
  dictionaries["Theme"] = theme;

  // Ici debut du nouveau fonctionnement
  list<list<string>> file = getFile("languages.csv");
  // Traitement des données pour qu elles soient exploitables en map<?>
  list<list<string>>::iterator it;
  list<string>::iterator it2;
  list<string> header;
  list<string> line;

  it = file.begin();
  line = (*it);
  header = (*it);
  for(it2 = ++line.begin(); it2 != line.end(); it2++) {
    map<string, string> dict;
    dict.insert(theme.begin(), theme.end());
    dictionaries[(*it2)] = dict;
    this->languages.push_back((*it2));
  }

  for(it = ++file.begin(); it != file.end(); it++) {
    line = (*it);

    list<string>::iterator ith;
    it2 = line.begin();
    string keyword = (*it2);

    for(ith = ++header.begin(); ith != header.end(); ith++) {
      dictionaries[(*ith)][keyword] = (*++it2);
    }
  }
  selectDictionary();
}

string Dictionary::translate(string key) {
  return dictionary[key];
}


