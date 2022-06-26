#include "Interactor.h"

using namespace Interactor;

Explorer::Explorer () : Behavior() {
}

void Explorer::compute (WildlifeModel * data) {
	// Mettre ici l'intelligence artificielle pour le comportement
  data->setX(data->getX() + data->random(-1, 1) % data->getWorld()->getWidth());
  data->setY(data->getY() + data->random(-1, 1) % data->getWorld()->getHeight());
}

