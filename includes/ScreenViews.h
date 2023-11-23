#ifndef SCREENVIEWS_H
#define SCREENVIEWS_H

#include "ScreenManager.h"
#include "Keyboards.h"

#include <ncurses.h>
#include <unistd.h>

using namespace ScreenManager;
using namespace keyboards;

namespace ScreenViews {

  /// class NCursesView -
  class NCurses : public ScreenView {
    public:
      // Screens
      const int MAIN_MENU = 0;
      const int SAVE_MENU = 1;
      const int LOAD_MENU = 2;
      const int IN_GAME   = 3;
      const int GAME_OVER = 4;

      // Menus
      const int NEW = 0;
      const int SAVE = 1;
      const int LOAD = 2;
      const int QUIT = 3;

    private:
      // Attributes
      WINDOW * main;
      WINDOW * window;
      int worldHeight = 0;
      int worldWidth = 0;
      int choice;
      int highlight = 0;
      keyboards::NCurses * keyb;
      int currentWindow = MAIN_MENU;

      // Operations
    public:
      NCurses (keyboards::NCurses * keyb);
      ~NCurses ();
      void init (int worldHeight, int worldWidth);
      void mainMenu ();
      void loadMenu (list<string> files);
      void infos (list<string> infos);
      void gameplay (ScreenViewModel * data);
      void end ();
      void hello (); // Pour tester l'affichage en cas de soucis
      void changeScreen (const int screen);
      int getScreen ();
      //*****************************************//
      void keyboardListener(WorldModel worldData);
  };

};

#endif
