#include "Interactor.h"
#include <random>

using namespace Interactor;


WildlifeModel::WildlifeModel () {
  turnsNumberBeforeStarving = defaultTurnsNumberBeforeStarving;
}

WildlifeModel::WildlifeModel (const WildlifeModel &wm) {
  name        = wm.name;
  lifetime    = wm.lifetime;
  maturityAge = wm.maturityAge;
  displayChar = wm.displayChar;
  viewField   = wm.viewField;
  XPosition   = wm.XPosition;
  YPosition   = wm.YPosition;
  path        = wm.path;
  movingTerrainType                = wm.movingTerrainType;
  defaultTurnsNumberBeforeStarving = wm.defaultTurnsNumberBeforeStarving;
  turnsNumberBeforeStarving        = wm.defaultTurnsNumberBeforeStarving;
}

string WildlifeModel::getRawData () {
  string separator = "|";
  string raw = "";

  raw = separator + name + separator;
  raw += to_string(lifetime) + separator;
  raw += to_string(maturityAge) + separator;
  raw += displayChar + separator;
  raw += to_string(viewField) + separator;

  raw += to_string(XPosition) + separator;
  raw += to_string(YPosition) + separator;

  //for each
  raw += "[";

  deque<pair<int, int>>::iterator it;
  for(it = path.begin(); it != path.end(); it++) {
    raw += "(" + to_string(it->first) + ":" + to_string(it->second) + ")";
    if(it + 1 != path.end())
      raw += ",";
  }

  raw += "]" + separator;

  raw += to_string(movingTerrainType) + separator;
  raw += to_string(defaultTurnsNumberBeforeStarving) + separator;
  raw += to_string(turnsNumberBeforeStarving) + separator;

  return raw.c_str();
}

char* WildlifeModel::getName() {
  return this->name;
}

int WildlifeModel::getX () {
  return XPosition;
}

int WildlifeModel::getY () {
  return YPosition;
}

int WildlifeModel::getFieldOfView () {
  return fieldOfView;
}

char WildlifeModel::getDisplayChar () {
  return displayChar;
}

const unsigned int WildlifeModel::getLifetimeRemaining () {
  return this->lifetime;
}

const unsigned int WildlifeModel::getAge () {
  return this->age;
}

const unsigned int WildlifeModel::getMaturityAge () {
  return this->maturityAge;
}

World * WildlifeModel::getWorld () {
  return this->world;
}

int WildlifeModel::getViewField() {
  return this->viewField;
}

int WildlifeModel::getTurnsNumberBeforeStarving () {
  return this->turnsNumberBeforeStarving;
}

void WildlifeModel::getHungry () {
  this->turnsNumberBeforeStarving--;
}

map<int, list<ClockSubscriber *>> WildlifeModel::getVision () {
  return vision;
}

int WildlifeModel::getMovingTerrainType () {
  return movingTerrainType;
}

void WildlifeModel::happyBirthday () {
  this->age++;
}

void WildlifeModel::setMaturityAge(unsigned int age) {
  this->maturityAge = age;
}

void WildlifeModel::setName (char * name) {
  this->name = name;
}

void WildlifeModel::setLifetime (int min, int max) {
  this->lifetime = random(min, max);
}

void WildlifeModel::setX (int x) {
  XPosition = x;
}

void WildlifeModel::setY (int y) {
  YPosition = y;
}

void WildlifeModel::setViewField (int v) {
  this->viewField = v;
}

void WildlifeModel::setDisplayChar (char c) {
  this->displayChar = c;
}

void WildlifeModel::setDefaultTurnsNumberBeforeStarving (int turns) {
  this->defaultTurnsNumberBeforeStarving = turns;
  this->turnsNumberBeforeStarving = turns;
}

void WildlifeModel::setWorld (World * w) {
  this->world = w;
}

void WildlifeModel::setFieldOfView (int distance) {
  this->fieldOfView = distance;
}

void WildlifeModel::setVision (map<int, list<ClockSubscriber *>> v) {
  this->vision = v;
}

void WildlifeModel::setMovingTerrainType (int terrainType) {
  movingTerrainType = terrainType;
}

int WildlifeModel::random (const int min, const int max) {
  random_device                  rand_dev;
  mt19937                        generator(rand_dev());
  uniform_int_distribution<int>  distr(min, max);

  return distr(generator);
}

void WildlifeModel::savePosition () {
  pair<int, int> pos(this->getX(), this->getY());
  path.push_back(pos);
  if(path.size() > 5) { // 5 est arbitraire pour le moment
    path.pop_front();
  }
}

bool WildlifeModel::isKnownedPosition (int posX, int posY) {
  pair<int, int> search(posX, posY);
  return find(path.begin(), path.end(), search) != path.end();
}

// Vision de 1 case
// 1 2 3
// 4 . 6
// 7 8 9
//
// Vision de 2 cases
//  1  2  3  4  5
//  6  7  8  9 10
// 11 12  . 14 15
// 16 17 18 19 20
// 21 22 23 24 25
//
// nb de cases = (distance vision * 2 + 1) ^ 2
//
// exemple une distance de vision de 4
//   ( 4 * 2 + 1) ^ 2 = 9 ^ 2 = 81
//
// Comment calculer la différence des positions ??
//
// . . . . .
// . . . . .
// . . * . .
// . o . . .
// . . . . .
//
// avec o l'objet de référence et * l'objet qui est 'vu'
// L'origine du tableau est en haut a gauche
//
// donc x = 2
// et y = 4
// curX = 3
// curY = 3
//
// La position de * dans le champ de vision est
//  x = x - CurX = 1
//  y = y - CurY = 1
//
map<int, list<ClockSubscriber *>> WildlifeModel::openYourEyes () {
  WorldModel worldData = this->getWorld()->getData();
  map<int, list<ClockSubscriber *>> vision = this->getVision();
  map<string, ClockSubscriber *>::iterator it;
  map<string, ClockSubscriber *> subscribers = worldData.getClock()->getSubscribers();

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    Wildlife * wl = (Wildlife *)it->second;

    int curX = wl->getX();
    int curY = wl->getY();
    int index = calculateIndex(XPosition - curX, YPosition - curY);

    if(XPosition - fieldOfView <= curX && curX <= XPosition + fieldOfView
        && YPosition - fieldOfView <= curY && curY <= YPosition + fieldOfView) {
      vision[index].push_back(it->second);
    }
  }
  return vision;
}

//=================
//  Maintenant il faudrait avoir l'inverse...
//
//  A partir des indices en déduire les coordonnees
//
// 1 2 3
// 4 . 6
// 7 8 9
//
// si j'ai l'indice 7
// la taille du block est 3
//
// 7 % 3 => 1
// si modulo = 0 => variable = taille de block
// 1 - 2 = -1 => position x
// avec 2 = distance + 1
//
// pour y:
// 7 - ( 7 % 3) = 6 / taille block = 2
// donc 2 blocks complets + modulo... ???
// si modulo > 0 : 2 + 1 = 3
//
// 3 - 2 = +1
//
// Vision de 2 cases
//  1  2  3  4  5
//  6  7  8  9 10
// 11 12  . 14 15
// 16 17 18 19 20
// 21 22 23 24 25
//
// test avec 2 cases...
//
// l'indice 10 donne :
// taille block 5
//
// 10 % 5 = 0
// le modulo est 0 donc on prends la taille du block 5
// distance * 2 = 4
// 5 - 3 = 2 (3 est la position centrale = distance + 1)
// donc x = +2
//
// pour y...
// 10 - (10 % 5) = 10 / 5 = 2
// milieu - res = pos
// 3 - 2 = +1
// donc y = +1
//

pair<int, int> WildlifeModel::calculateCoordinates (int index) {
  pair<int, int> position;

  const unsigned int centralPosition = fieldOfView + 1;
  const unsigned int blockSize = fieldOfView * 2 + 1;

  // Calcul de la position X
  unsigned int modulo = index % blockSize;
  if(modulo == 0) {modulo = blockSize;}
  position.first = modulo - centralPosition;

  //calcul de la position Y
  position.second = centralPosition - (index - modulo / blockSize);
  modulo = index % blockSize;
  if(modulo > 0) {position.second++;}

  return position;
}

// Vision de 2 cases
//  1  2  3  4  5
//  6  7  8  9 10
// 11 12  . 14 15
// 16 17 18 19 20
// 21 22 23 24 25
//
// calculer les positions dans le tableau
// avec les indices et/ou les coordonnees
//
// exemple pour une distance de 1, la position x, y : +1, -1
// par experience on sait qu'il s'agit de la position 3
// comment la calculer ???
//
//  avec les coordonnees :
//
//  on a la taille d'une ligne ou taille de bloc en faisant ce calcul
//  taille block = distance vision * 2 + 1
//
//  5 - 2 = 3
//  5 - taille block => 5 - 3 = 2 (pour Y en premier)
//
//  2 + 1 = 3 => 2 + position x = 3
//
// pour calculer la position d'origine qui se situe au centre
//
//  (taille du tableau + 1) / 2
//  (9 + 1) / 2 = 10 / 2 = 5
//
//  dans la cas d'une distance de 2, on a :
//  (25 + 1) / 2 = 26 / 2 = 13
//
// Vision de 1 case
// 1 2 3
// 4 . 6
// 7 8 9
//
// Exemple avec la position +1, +1
// trouver le centre en premier ??
// centre = ( taille + 1 ) / 2
// on calcul avec y
// blockSize = 1 * 2 + 1 = 3
// resIndex = centre + (y * blockSize) + x

unsigned int WildlifeModel::calculateIndex (pair<int, int> position) {
  const unsigned int blockSize = fieldOfView * 2 + 1;
  const unsigned int size = pow(blockSize, 2);
  const unsigned int centralIndex = (size + 1) / 2;

  return centralIndex + position.first + (position.second * blockSize);
}

unsigned int WildlifeModel::calculateIndex (int x, int y) {
  pair<int, int> position;
  position.first = x;
  position.second = y;

  return calculateIndex(position);
}

list<string> WildlifeModel::log () {
  list<string> messages = Loggable::log();

  string n(name);
  messages.push_back("Name : " + n);
  messages.push_back("Lifetime : " + to_string(lifetime));
  messages.push_back("Age : " + to_string(age));
  messages.push_back("DisplayChar : " + to_string(displayChar));
  messages.push_back("View Field : " + to_string(viewField));
  messages.push_back("X position : " + to_string(XPosition));
  messages.push_back("Y position : " + to_string(YPosition));
  messages.push_back("Is knowned position ? : " + to_string(isKnownedPosition(XPosition, YPosition)));
  messages.push_back("Default turns before starving : " + to_string(defaultTurnsNumberBeforeStarving));
  messages.push_back("Remaining turns before starving : " + to_string(turnsNumberBeforeStarving));
  messages.push_back("Field of view : " + to_string(fieldOfView));
  messages.push_back("Moving terrain type : " + to_string(movingTerrainType));
  deque<pair<int, int>>::iterator it;
  string line = "";
  for(it = path.begin(); it != path.end(); it++) {
    //line += "->";
    messages.push_back("x: " + to_string(it->first) + ", y: " + to_string(it->second));
  }

  map<int, int> worldMap = this->world->getData().getWorldMap();
  int index = calculateIndex(XPosition, YPosition);
  messages.push_back("WorldMap index : " + to_string(index));
  messages.push_back("WorldMap terrain type : " + to_string(worldMap[index]));

  messages.push_back(line);
  /*  map<int, list<ClockSubscriber *>>::iterator it2;
      for(it2 = vision.begin(); it2 != vision.end(); it2++) {
      line = to_string(it2->first) + "(" + to_string(it2->second.size()) + ")";
      messages.push_back(line);
      }
      */

  return messages;
}

