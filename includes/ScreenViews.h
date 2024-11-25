#ifndef SCREENVIEWS_H
#define SCREENVIEWS_H

#include "ScreenManager.h"

#include <ncurses.h>
#include <unistd.h>

using namespace ScreenManager;

namespace ScreenViews {

  /// class NCursesView -
  class NCurses : public ScreenView {
    private:
      // Attributes
      WINDOW * windows[9];
      int worldHeight = 1;
      int worldWidth = 1;
      int choice;
      int highlight = 0;
      int currentWindow = MAIN;
      ScreenViewModel * data;
      int windowHeight = 0;
      int windowWidth = 0;

      WINDOW * createWindow (int height = 2, int width = 10);
      void redraw (WINDOW * mainMenu);
      void drawChar (WINDOW * win, int x, int y, char c, char color);
      //TODO: faire remonter le static dans l objet ScreenView
      static void display (WINDOW * mainMenu, ScreenViewModel * data, int keybPosition = -1);


      // Operations
    public:
      NCurses (Keyboard * keyb);
      ~NCurses ();
      void init (ScreenViewModel * data);
      WINDOW * getWindow ();
      void displayCursorPosition (int keybPosition);
      static void logCursorPosition (int keybPosition);
      void mainMenu (int keybPosition = -1);
      void options (int keybPosition = -1);
      void languages (int keybPosition = -1);
      void video (int keybPosition = -1);
      void save (list<string> files, const unsigned int menuSize, int keybPosition = -1);
      void load (list<string> files, const unsigned int menuSize, int keybPosition = -1);
      void infos (list<string> infos);
      void gameplay ();
      void end ();
      int getChar ();
      void hello (); // Pour tester l'affichage en cas de soucis
      void validateOption (int optionNumber);
      const unsigned int computeMaxWidth (list<Node *> menu);
  };

};

#endif
