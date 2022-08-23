#include "Interactor.h"

using namespace Interactor;

Explorer::Explorer () : Behavior() {
}

void Explorer::compute (WildlifeModel * data) {
	// Mettre ici l'intelligence artificielle pour le comportement
  data->savePosition();

  vector<int> pos = this->getNewPosition(data);

  data->setX(pos[0]); //x
  data->setY(pos[1]); //y
}

//TODO : A ecrire mieux que ca...
vector<int> Explorer::getNewPosition(WildlifeModel * data) {
  vector<int> position;

  // 1 2 3
  // 4 . 5
  // 6 7 8

  int x = data->getX();
  int y = data->getY();
  int width = data->getWorld()->getWidth();
  int height = data->getWorld()->getHeight();
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
  }

  int index = data->random(0, possibles.size()-1);
  pair<int, int> newPos = possibles[index];

  position.push_back(newPos.first);
  position.push_back(newPos.second);

  return position;
}

