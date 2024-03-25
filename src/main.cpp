#include "ScreenManager.h"
#include "Interactor.h"
#include "Sound.h"

#include "Keyboards.h"
#include "ScreenViews.h"

#include <thread>

using namespace ScreenManager;
using namespace Interactor;
using namespace sound;

// Prototypes
void display(World * w);

int main(int argc, char** argv) {
  // Let's play the music ....
  Music m;
  std::thread t_m(&Music::play);

  // creation d'un monde 20x30
  World * w = new World(20, 30);
  /*
     for(int i = 0; i < 10; i++) {
     new Fish(w);
     }
     */
  Logger * log = new Logger("/var/log/ecosystem/mon_fichier.log");

  new Fish(w);
  //new Shark(w);

  WorldModel d = w->getData();

  log->log(&d);
  display(w);

  return 0;
}

void display(World * w) {
  ScreenViewModel * svm = new ScreenViewModel(w->getData());
  Keyboards::NCurses * kb = new Keyboards::NCurses();
  ScreenViews::NCurses * sv = new ScreenViews::NCurses(kb);

  OutputDevice * oDevice = new ScreenPresenter(w, svm, kb, sv);
  oDevice->display();
}

