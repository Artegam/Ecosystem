#include "Interactor.h"
#include <random>

using namespace Interactor;


WildlifeModel::WildlifeModel () {
  turnsNumberBeforeStarving = defaultTurnsNumberBeforeStarving;
}

WildlifeModel::WildlifeModel (const WildlifeModel &wm) {
  name        = wm.name;
  lifetime    = wm.lifetime;
  age         = wm.age;
  displayChar = wm.displayChar;
  viewField   = wm.viewField;
  XPosition   = wm.XPosition;
  YPosition   = wm.YPosition;
  path        = wm.path;
  defaultTurnsNumberBeforeStarving = wm.defaultTurnsNumberBeforeStarving;
  turnsNumberBeforeStarving        = wm.defaultTurnsNumberBeforeStarving;
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

char WildlifeModel::getDisplayChar () {
  return displayChar;
}

const unsigned int WildlifeModel::getLifetimeRemaining () {
  return this->lifetime;
}

const unsigned int WildlifeModel::getAge () {
  return this->age;
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

void WildlifeModel::happyBirthday () {
  this->age++;
}

void WildlifeModel::setName(char * name) {
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

void WildlifeModel::makeOld() {
}

int WildlifeModel::random (const int min, const int max) {
  random_device                  rand_dev;
  mt19937                        generator(rand_dev());
  uniform_int_distribution<int>  distr(min, max);

  return distr(generator);
}

void WildlifeModel::savePosition() {
  string pos = this->getX() + "-" + this->getY();
  path.insert(path.begin(), pos);
  if(path.size() > 5) { // 5 est arbitraire pour le moment
    path.pop_back();
  }
}

bool WildlifeModel::isKnownedPosition(int posX, int posY){
  vector<string>::iterator itr;
  string search = posX + "-" + posY;
  itr = find(path.begin(), path.end(), search);

  return false;
}

