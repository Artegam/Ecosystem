#ifndef KEYBOARDS_H
#define KEYBOARDS_H

#include <ncurses.h>
#include "ScreenManager.h"

using namespace ScreenManager;

namespace Keyboards {

  class NCurses : public Keyboard {
  //class NCurses {
    // Attributes
    private:
      WINDOW * win;
    public:
      NCurses ();
      void listen (unsigned int character);
      void setWindow (WINDOW * win);
  };

};

#endif
