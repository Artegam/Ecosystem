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
      const int MENU_MAIN    = 0;
      const int MENU_OPTIONS = 1;
      const int MENU_SAVE    = 2;
      const int MENU_LOAD    = 3;
      const int IN_GAME      = 4;
      const int GAME_OVER    = 5;

      // Menus
      const int NEW     = 0;
      const int OPTIONS = 1;
      const int SAVE    = 2;
      const int LOAD    = 3;
      const int QUIT    = 4;

    private:
      // Attributes
      WINDOW * main;
      WINDOW * window;
      int worldHeight = 0;
      int worldWidth = 0;
      int choice;
      int highlight = 0;
      keyboards::NCurses * keyb;
      int currentWindow = MENU_MAIN;
      ScreenViewModel * data;
      int windowHeight = 0;
      int windowWidth = 0;

      WINDOW * subMenuInit (int size = 2, int width = 10);
      void listenKeyboard (WINDOW * mainMenu, int size = 2);
      void subMenu (int width, list<string> items);
      void redraw (WINDOW * mainMenu);


      // Operations
    public:
      NCurses (keyboards::NCurses * keyb);
      ~NCurses ();
      void init (ScreenViewModel * data);
      void mainMenu ();
      void options (list<string> options);
      void load (list<string> files);
      void infos (list<string> infos);
      void gameplay ();
      void end ();
      void hello (); // Pour tester l'affichage en cas de soucis
      void changeScreen (const int screen);
      int getScreen ();
      //*****************************************//
      void keyboardListener(WorldModel worldData);
  };

};

#endif
