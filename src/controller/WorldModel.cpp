#include "Controller.h"

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
  unsigned int size = width * (height + 1);

  for(unsigned int index = 0; index < size; index++) {
    worldMap[index] = OCEAN;
	}
}

pair<int, int> WorldModel::calculateCoordinates (int index) {
	pair<int, int> position;

  const unsigned int centralPosition = width + 1;
  const unsigned int blockSize = width * 2 + 1;

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

unsigned int WorldModel::calculateIndex (pair<int, int> position) {
  const unsigned int blockSize = width * 2 + 1;
  const unsigned int size = pow(blockSize, 2);
	const unsigned int centralIndex = (size + 1) / 2;

  return centralIndex + position.first + (position.second * blockSize); 
}

unsigned int WorldModel::calculateIndex (int x, int y) {
  pair<int, int> position;
	position.first = x;
	position.second = y;

	return calculateIndex(position);
}

