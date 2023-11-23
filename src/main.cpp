#include "ScreenViews.h"
#include "Keyboards.h"
#include "Interactor.h"


using namespace ScreenViews;
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
  //Logger * log = new Logger("bin/mon_fichier.log");
  Logger * log = new Logger("/var/log/ecosystem/mon_fichier.log");

  new Fish(w);
  //new Shark(w);

  WorldModel d = w->getData();

  log->log(&d);
  display(w);

  return 0;
}

void display(World * w) {
  keyboards::NCurses * keyb = new keyboards::NCurses();
  ScreenViews::NCurses * view = new ScreenViews::NCurses(keyb);
  ScreenManager::ScreenPresenter * sp = new ScreenManager::ScreenPresenter(view, w);
  sp->display();
}

