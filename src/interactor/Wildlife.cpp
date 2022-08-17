#include "Interactor.h"
#include <cstring>

using namespace Interactor;
using namespace Controller;
using namespace std;

Wildlife::Wildlife () {
  data = new WildlifeModel();
}

/*
Wildlife::Wildlife (const Wildlife * w) {
  name        = w->name;
  lifetime    = w->lifetime;
  age         = w->age;
  displayChar = w->displayChar;
  viewField   = w->viewField;
  XPosition   = w->XPosition;
  YPosition   = w->YPosition;
  action      = w->action;
  world       = w->world;
}
*/

Wildlife::Wildlife (World * world) {
  data = new WildlifeModel();
  data->setWorld(world);
  data->setX(data->random(1, world->getWidth()));
  data->setY(data->random(1, world->getHeight()));
  Clock * c = world->getClock();
  c->subscribe(this);
}

Wildlife::Wildlife (WildlifeModel * parentData) {
  World * world = parentData->getWorld();
  data = new WildlifeModel();
  data->setWorld(world);
  data->setX(parentData->getX());
  data->setY(parentData->getY());
  Clock * c = world->getClock();
  c->subscribe(this);
}

Wildlife::~Wildlife () {
}

void Wildlife::execute () {
  printf("Je m'execute en Wildlife...\n");
}

void Wildlife::update () {
  //printf("Je me mets a jour en Wildlife...\n");
  action->compute(data);


//############################
	//TODO: Faire un tableau pour la vue du poisson
  /*
	int originX = this->getX() - this->data->getViewField();// *2 + 1;
	int originY = this->getY() - this->data->getViewField();
	int endX = originX * 2 + 1;
	int endY = originY * 2 + 1;

	//World world = *data->getWorld();

	for (int x = originX; x <= endX; x++) {
		for (int y = originY; y <= endY; y++) {
			char key[10] = "";
			sprintf(key, "%d-%d", x, y);
			//vision.insert({key, world->map[x][y].size()});
		}
	}
  */
/*
	// Pour utiliser la map vision faire des tris etc...
	//https://www.developpez.net/forums/d1047898/c-cpp/cpp/bibliotheques/sl-stl/ordonner-elements-d-map/

//#### TODO: la partie de tri et de prendre la position vers où aller => à mettre dans l'objet Behavior
  std::sort(vision.begin(), vision.end(), this->cmp);
	//Map vision triée

	printf("La plus grosse position connue est : %s, avec la valeur %d\n", vision.begin()->first, vision.begin()->second);

  int pos_delim = vision.begin()->first.find("-");
	string x = vision.begin()->first.substr(0, pos_delim);
	string y = vision.begin()->first.substr(pos_delim + 1);

	printf("La position est x: %d y: %d\n", atoi(x), atoi(y));

//###########################
*/

  this->makeOld();
}

template <class T> bool Wildlife::cmp(pair<T, T>& x1, pair<T, T>& x2) {
  // return type is bool
  return x1.second <= x2.second;
}

void Wildlife::makeOld() {
  data->makeOld();
  data->happyBirthday();
  data->getHungry();

  if(isDead() || isStarving()) {
    this->die = true;
    data->getWorld()->getClock()->unsubscribe(this);
  }
}

const unsigned int Wildlife::getAge () {
  return data->getAge();
}

const unsigned int Wildlife::getLifetimeRemaining () {
  return data->getLifetimeRemaining();
}

int Wildlife::getX () {
  return data->getX();
}

int Wildlife::getY () {
  return data->getY();
}

char Wildlife::getDisplayChar () {
  return data->getDisplayChar();
}

void Wildlife::addWildlife(string wildlifeName, int number) {
	for(int i = 1; i < number; i++) {
    if(!strcmp(wildlifeName.c_str(), "Fish")) {
      new Fish(data);
    } else if (!strcmp(wildlifeName.c_str(), "Shark")) {
      new Shark(data);
    }
	}
}

bool Wildlife::isStarving () {
  return (data->getTurnsNumberBeforeStarving() == 0);
}

bool Wildlife::isDead () {
  return (data->getLifetimeRemaining() <= 0);
}
