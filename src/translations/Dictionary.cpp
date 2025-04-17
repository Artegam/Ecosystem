#include "Translations.h"

using namespace std;
using namespace Translations;

Dictionary::Dictionary () {
  loadDictionaries();
  language = languages.front();
  selectDictionary();
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

string Dictionary::getLanguage () {
  return language;
}

void Dictionary::setLanguage (const int index) {
  list<string>::iterator itl = languages.begin();
  advance(itl, index);
  language = (*itl);
  selectDictionary();
}

void Dictionary::setLanguage (string lang) {
  language = lang;
  selectDictionary();
}

void Dictionary::selectDictionary() {
  // Selection du dictionnaire et retour
  //list<string>::iterator itl = this->languages.begin();
  //advance(itl, this->language);

  //dictionary = dictionaries[(*itl)];
  dictionary = dictionaries[language];
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
    languages.push_back((*it2));
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

string Dictionary::translate(string sentence) {

  // ceci implique que toutes les phrases soit ecrites avec les cles des dictionnaires

  // decouper la chaine de caracteres sur les carateres espace
  // chercher toutes les positions de charactere espace et mettre ca dans une liste
  size_t begin = sentence.find_first_not_of(' ');
  size_t end = sentence.find_last_not_of(' ');

  list<unsigned int> positions;
  size_t found = begin;
  while((found = sentence.find(" ", found)) != string::npos && found <= end) {
    positions.push_back(found);
    found++;
  }

  // si il n'y a pas d'espace on a un mot
  if(positions.size() == 0) {
    if(dictionary[sentence] != "")
      return dictionary[sentence];
    else
      return sentence;
  }

  // calculer le nombre de mots
  //unsigned int wordsNumber = positions.size() + 1;

  // calculer la longueur de chaque mots
  list<pair<unsigned int, unsigned int>> words; // pair<premier charactere, longueur>
  list<unsigned int>::iterator pos = positions.begin();
  list<unsigned int>::iterator lastPos;
  for(unsigned int i = 0; i <= positions.size(); i++) {
    if (i == 0) //premier mot
      words.push_back(make_pair(begin, *pos - begin));
    if (i > 0 && i < positions.size())
      words.push_back(make_pair(*lastPos + 1, *pos - *lastPos + 1));
    if (i == positions.size()) // dernier mot
      words.push_back(make_pair(*lastPos + 1, end - *lastPos));
    lastPos = pos;
    pos++;
  }

  // traduire le mot dans la langue selectionee et reconstituer la chaine
  list<pair<unsigned int, unsigned int>>::reverse_iterator word;
  for(word = words.rbegin(); word != words.rend(); ++word) {
    string key = sentence.substr((*word).first, (*word).second);
    if(dictionary[key] != "")
      sentence.replace((*word).first, (*word).second, dictionary[key]);
  }

  // renvoyer la chaine
  return sentence;
}


