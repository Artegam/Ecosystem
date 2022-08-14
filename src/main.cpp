#include "ConsoleView.h"
#include "Interactor.h"

using namespace Controller;
using namespace ConsoleView;
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
  new Fish(w);
  new Shark(w);
  display(w);

	return 0;
}

void display(World * w) {
  NCursesView * view = new NCursesView();
  ScreenPresenter * sp = new ScreenPresenter(view, w);
  sp->display();
}

