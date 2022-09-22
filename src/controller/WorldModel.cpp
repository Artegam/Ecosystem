#include "Controller.h"
#include <random>
#include <set>

using namespace Controller;

WorldModel::WorldModel () {
  init();
}

WorldModel::WorldModel (int width, int height) {
  init();
  this->width = width;
	this->height = height;
  generateMap ();
}

void WorldModel::init() {
  this->clock = new Clock();
}

void WorldModel::setWidth(int w) {
  width = w;
}

void WorldModel::setHeight(int h) {
  height = h;
}

void WorldModel::setRefresh(unsigned int interval) {
  this->clock = new Clock(interval);
}

int WorldModel::getWidth (void) {
  return width;
}

int WorldModel::getHeight (void) {
  return height;
}

Clock * WorldModel::getClock () {
  return clock;
}

map<int, int> WorldModel::getWorldMap () {
  return worldMap;
}

void WorldModel::generateMap () {
  unsigned int const OCEAN = 0;
  unsigned int const PLAIN = 1;
  unsigned int size = width * height;

  for(unsigned int index = 0; index < size; index++) {
		worldMap[index] = OCEAN;
  }

  unsigned int plainBoxNumber = round(size * 0.33);

  set<int> plainBoxes;

  while(plainBoxes.size() < plainBoxNumber) {
		plainBoxes.insert(random(0, (size - plainBoxes.size())));
	}

	set<int>::iterator it;
  for(it = plainBoxes.begin(); it != plainBoxes.end(); it++) {
    worldMap[*it] = PLAIN;
	}

}

pair<int, int> WorldModel::calculateCoordinates (int index) {
	pair<int, int> position;

  //const unsigned int centralPosition = width + 1;
  //const unsigned int blockSize = width * 2 + 1;

  // Calcul de la position X
  /*
  unsigned int modulo = index % blockSize;
  if(modulo == 0) {modulo = blockSize;}
  position.first = modulo - centralPosition;*/
  position.first = index % width;

  //calcul de la position Y
  /*
  position.second = centralPosition - (index - modulo / blockSize);
  modulo = index % blockSize;
  if(modulo > 0) {position.second++;}
  */
  position.second = (index - position.first) / width;

	return position;
}

unsigned int WorldModel::calculateIndex (pair<int, int> position) {
  return position.first + position.second * width;
}

unsigned int WorldModel::calculateIndex (int x, int y) {
  pair<int, int> position;
	position.first = x;
	position.second = y;

	return calculateIndex(position);
}

int WorldModel::random (const int min, const int max) {
  random_device                  rand_dev;
  mt19937                        generator(rand_dev());
  uniform_int_distribution<int>  distr(min, max);

  return distr(generator);
}

list<string> WorldModel::log() {

  list<string> messages = Loggable::log();
  messages.push_back("appel a WorldModel::log()");

  string str;

  messages.push_back("Height : " + to_string(height));
  messages.push_back("Width : " + to_string(width));
  messages.push_back("World Map :"); 
  map<int, int>::iterator it;
  string line;
	for(it = worldMap.begin(); it != worldMap.end(); it ++) {
    line += to_string(it->second);
    if(it->first % width == 0) {
			messages.push_back(line);
      line = "";
		}
	}

  return messages;
}
