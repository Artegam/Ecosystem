#include "Interactor.h"

using namespace Interactor;

Default::Default () : Behavior() {
}

void Default::compute (WildlifeModel * data) {
	// Mettre ici l'intelligence artificielle pour le comportement
  data->savePosition();

  vector<int> pos = this->getNewPosition(data);

  data->setX(pos[0]); //x
  data->setY(pos[1]); //y
}

vector<int> Default::getNewPosition(WildlifeModel * data) {
  vector<int> position = {0, 0};

  int posX = (data->getX() + data->random(-1, 1)) % data->getWorld()->getWidth();
  int posY = (data->getY() + data->random(-1, 1)) % data->getWorld()->getHeight();

  if(!posX) {posX++;}
  if(!posY) {posY++;}

  position[0] = posX;
  position[1] = posY;

  return position;
}

