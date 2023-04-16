#include "Views.h"
#include "Interactor.h"


using namespace Controller;
using namespace Views;
using namespace Interactor;


// Prototypes
void display(World * w);

int main(int argc, char** argv) {

  // creation d'un monde 20x30
  World * w = new World(20, 30);
  /*
  for(int i = 0; i < 10; i++) {
    new Fish(w);
  }
  */
  Logger * log = new Logger("bin/mon_fichier.log");

  new Fish(w);
  //new Shark(w);

  WorldModel d = w->getData();

  log->log(&d);
  display(w);

	return 0;
}

void display(World * w) {
  NCursesView * view = new NCursesView();
  ScreenPresenter * sp = new ScreenPresenter(view, w);
  sp->display();
}

