#include "Interactor.h"

using namespace Interactor;

Survival::Survival () : Behavior() {
}

void Survival::compute (WildlifeModel * data) {
	// Mettre ici l'intelligence artificielle pour le comportement
  data->savePosition();

  vector<int> pos = this->getNewPosition(data);

  data->setX(pos[0]); //x
  data->setY(pos[1]); //y
}

vector<int> Survival::getNewPosition(WildlifeModel * data) {
  vector<int> position;

  // 1 2 3
  // 4 . 5
  // 6 7 8

  int x = data->getX();
  int y = data->getY();
  WorldModel worldData = data->getWorld()->getData();
  int width = worldData.getWidth();
  int height = worldData.getHeight();
  // La map weight possede le poid en clé pour le tri et l'index associe de la map de vision
  map<int, int> weight;
	map<int, list<ClockSubscriber *>> myVision = data->getVision();

  vector<pair<int,int>> possibles = {
    { (width + (x-1)) % width, (height + (y-1)) % height},
    { x                      , (height + (y-1)) % height},
    { (x+1) % width          , (height + (y-1)) % height},
    { (width + (x-1)) % width, y},
    { (x+1) % width          , y},
    { (width + (x-1)) % width, (y+1) % height},
    { x                      , (y+1) % height},
    { (x+1) % width          , (y+1) % height},
  };

  std::vector<std::pair<int,int>>::iterator it;
  for(it = possibles.begin(); it != possibles.end(); it++) {
    if(data->isKnownedPosition(it->first, it->second)) {
      possibles.erase(it);
    }
    int index = data->calculateIndex(it->first, it->second);
    myVision.erase(index);
  }

  std::map<int, list<ClockSubscriber *>>::iterator itv;

	for(itv = myVision.begin(); itv != myVision.end(); itv++) {
		if(itv->second.size() > 0) {
			weight[itv->second.size()] = itv->first;
		}
	}

  // l'id du poid le plus fort est : weight.end()->second
  int index = weight.end()->second;
  pair<int, int> newPos = possibles[index];

  position.push_back(newPos.first);
  position.push_back(newPos.second);

  return position;
}


