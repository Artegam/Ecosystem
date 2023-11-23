#include "Keyboards.h"

#include <unistd.h>

using namespace keyboards;

NCurses::NCurses () {
}

void NCurses::setWindow(WINDOW * win) {
  this->win = win;
}

void NCurses::listen () {

  int choice = wgetch(win);

  switch(choice)
  {
    case KEY_UP:
      mvprintw(24, 0, "KEY_UP");
      onUp();
      break;
    case KEY_DOWN:
      mvprintw(24, 0, "KEY_DOWN");
      onDown();
      break;
    case '\n':
      mvprintw(24, 0, "KEY_ENTER");
      onEnter();
      break;
  }

  /*
  int c = getch();

  //WorldModel worldData = this->data->getWorldData();
  Clock * cl = worldData.getClock();

  switch(c)
  {	case KEY_UP:
      break;
    case KEY_DOWN:
      break;
    case 115:// 's' pour save
      if(cl->isRunning())
        cl->stop();
      //data->save();
      mvprintw(24, 0, "SAVED");
      break;
    case 108:// 'l' pour load
      if(cl->isRunning())
        cl->stop();
      //data->load();
      mvprintw(24, 0, "LOADED");
      break;
    case 112: // 'p' pour la pause
      if(cl->isRunning()) {
        cl->stop();
        mvprintw(24, 0, "PAUSE");
      } else {
        cl->run();
        mvprintw(24, 0, "PLAY");
      }
      break;
    default:
      mvprintw(24, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", c, c);
      break;
  }
  refresh();
  */

}

void NCurses::onUp () {
  if(positionSelected > 0)
    positionSelected--;
}

void NCurses::onDown () {
  if(positionSelected < this->positionsCount - 1)
    positionSelected++;
}

void NCurses::onEnter () {
  valid = true;
}

void NCurses::resetValid () {
  valid = false;
}

bool NCurses::isValid () {
  return valid;
}

void NCurses::setPositionsCount (const unsigned int count) {
  this->positionsCount = count;
}

void NCurses::setDefaultPosition () {
  positionSelected = 0;
}

void NCurses::setDefaultPosition (const unsigned int position) {
  positionSelected = position;
}

int NCurses::getPositionSelected () {
  return positionSelected;
}

