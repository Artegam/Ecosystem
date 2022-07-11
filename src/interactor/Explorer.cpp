#include "Interactor.h"

using namespace Interactor;

Explorer::Explorer () : Behavior() {
}

void Explorer::compute (WildlifeModel * data) {
	// Mettre ici l'intelligence artificielle pour le comportement

  int posX = (data->getX() + data->random(-1, 1)) % data->getWorld()->getWidth();
  int posY = (data->getY() + data->random(-1, 1)) % data->getWorld()->getHeight();

  if(!posX) {posX++;}
  if(!posY) {posY++;}

  data->setX(posX);
  data->setY(posY);
}

