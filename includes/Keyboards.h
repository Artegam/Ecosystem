#ifndef KEYBOARDS_H
#define KEYBOARDS_H

#include "ScreenManager.h"
#include <ncurses.h>

using namespace ScreenManager;

namespace keyboards {

  class NCurses {
    // Attributes
    private:
      WINDOW * win;
      int positionSelected = 0;
      int positionsCount = 1;
      bool valid = false;
    public:
      NCurses ();
      void listen ();
      void setWindow (WINDOW * win);
      void onUp ();
      void onDown ();
      void onEnter ();
      void resetValid ();
      bool isValid ();
      void setPositionsCount (const unsigned int count);
      void setDefaultPosition ();
      void setDefaultPosition (const unsigned int position);
      int getPositionSelected ();
  };

};

#endif
