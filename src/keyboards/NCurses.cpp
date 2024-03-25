#include "Keyboards.h"

#include <unistd.h>

using namespace Keyboards;

Keyboards::NCurses::NCurses () {
}

void Keyboards::NCurses::setWindow(WINDOW * win) {
  this->win = win;
}

void Keyboards::NCurses::listen (unsigned int character) {

  switch(character)
  {
    case KEY_UP:
      onUp();
      break;
    case KEY_DOWN:
      onDown();
      break;
    case '\n':
      onEnter();
      break;
  }

}

