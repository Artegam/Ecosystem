#include "Interactor.h"
#include <cstring>
#include <math.h>

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
  openYourEyes();
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

void Wildlife::openYourEyes () {
  int x = this->getX();
  int y = this->getY();
//  printf("pos %d - %d\n", x, y);
//  // TODO: Ici ca appel le getX et getY de clockSubscriber alors qu'il s'agit d'une interface
//  //       Il faut donc reflechir pour recuperer les donnees...
//  vector<vector<ClockSubscriber*>> myVision = data->getWorld()->getMap(x - 1, x + 1, y - 1, y + 1);
//
//  for(int curX = (x - 1); curX < (x + 1); curX++) {
//    for(int curY = (y - 1); curY < (y + 1); curY++) {
//      printf("%d - %d : %s\n", curX, curY, myVision[curX][curY]->getId().c_str());
//    }
//  }

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

  const unsigned int dist = 1;
  const unsigned int size = pow(dist * 2 + 1, 2);

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
// 1 - 2 = -1 => position x
// avec 2 = distance * 2 => A VERIFIER
//
// pour y:
// 7 - ( 7 % 3) = 6 / taille block = 2
// donc 2 blocks complets + modulo... ???
//
  printf("X: %d\n", x);
  printf("Y: %d\n", y);
  printf("size: %d\n", size);

  list<Wildlife *> lst;
  vector<vector<int>> myVision;

  vector<vector<int>> vis;

/*
  map<string, ClockSubscriber *>::iterator it;
  map<string, ClockSubscriber *> subscribers = data->getWorld()->getClock()->getSubscribers();

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    Wildlife * wl = (Wildlife *)it->second;
    int curX = wl->getX();
    int curY = wl->getY();
    if(curX > x - 1 && curX < x + 1
       && curY > y - 1 && curY < y + 1) {
      lst.push_back((Wildlife *)it->second);
      if(!myVision[curX][curY]) {
        printf("rien dans la position recherchee\n");
      }
      //myVision[curX][curY]++;
    }
  }
*/
}

bool Wildlife::isStarving () {
  return (data->getTurnsNumberBeforeStarving() == 0);
}

bool Wildlife::isDead () {
  return (data->getLifetimeRemaining() <= 0);
}
