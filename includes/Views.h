#ifndef VIEWS_H
#define VIEWS_H

#include "GraphicComponents.h"
#include "Translations.h"

#include <ncurses.h>
#include <unistd.h>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <thread>

using namespace std;
using namespace GraphicComponents;
using namespace Translations;

namespace Views {
  // Keyboard keys
  const int KEYB_DOWN   = 2;
  const int KEYB_UP     = 3;
  const int KEYB_ENTER  = 10;
  const int KEYB_SPACE  = 32;
  const int KEYB_ESCAPE = 27;


  // interface
  /// class Presenter - 
  class Presenter {
    private:
      void * fct;
    public:
      virtual void display ();
  };

  // interface
  /// class View - 
  class View {
    private:

    protected:
      bool toClear = false;
      Presenter * _presenter;
      int _keyboardx = 0;
      int _keyboardy = 0;
      int _maxKeyboardx = 0;
      int _maxKeyboardy = 0;
      bool _valid = false;
      Screen scr;
      Dictionary dict;

    // Operations
    public:

      View (Presenter * presenter);
      virtual void init (int height, int width);
      virtual void display ();
      virtual void display (Screen screen);
      virtual void display (Menu menu);
      virtual void display (Text text);
      virtual void mainMenu (int keybPosition = -1);
      virtual void options (int keybPosition = -1);
      virtual void languages (int keybPosition = -1);
      virtual void video (int keybPosition = -1);
      virtual void gameplay (int position, map<int, int> worldMap);
      virtual void end ();
      virtual int getChar ();
      virtual void clearScreen ();
      virtual void validateOption (int optionNumber);
      virtual void createWindow (int position, int x, int y, int height, int width);
      void keyboard ();
      const int getKeyboardx ();
      const int getKeyboardy ();
      void setMaxKeyboardx(const int max = 0);
      void setMaxKeyboardy(const int max = 0);
      void resetKeyboard();
      bool isValid ();
      void setLanguage(const unsigned int lang);
  };


  /// class NCursesView -
  class NCurses : public View {
    private:
      // Attributes
      int worldHeight = 1;
      int worldWidth = 1;
      int choice;
      int highlight = 0;
      int windowHeight = 0;
      int windowWidth = 0;
      void redraw (WINDOW * mainMenu);
      void drawChar (WINDOW * win, int x, int y, char c, char color);

    protected:
      bool toClear = true;
      vector<WINDOW*> windows;

      // Operations
    public:
      NCurses (Presenter * presenter);
      ~NCurses ();
      void init (int height, int width);
      void createWindow (int screen, int x, int y, int height = 2, int width = 10);
      void displayCursorPosition (int keybPosition);
      static void logCursorPosition (int keybPosition);
      void gameplay (int position, map<int, int> worldMap);
      void end ();
      int getChar ();
      void hello (); // Pour tester l'affichage en cas de soucis
      void validateOption (int optionNumber);
      void clearScreen();
      void initScreen();
      void display ();
      void display (Screen screen);
      void display (Menu menu);
      void display (Text text);
      void keyboard ();
  };

  /// class OpenGLView -
  class OpenGL : public View {
    private:
      // Attributes
      int worldHeight = 1;
      int worldWidth = 1;
      int choice;
      int highlight = 0;
      int windowHeight = 0;
      int windowWidth = 0;
      static vector<WINDOW*> windows;
      static Screen scr;
      static int _keyboardx;
      static int _maxKeyboardx;
      static Presenter _presenter;
      static Dictionary dict;

    protected:
      static int _maxKeybx;

      WINDOW * createWindow (int height = 2, int width = 10);
      void redraw (WINDOW * mainMenu);
      void drawChar (WINDOW * win, int x, int y, char c, char color);
      static void displayRoutine (void);
      static void keyboard (unsigned char key, int x, int y);
      static void special (int key, int x, int y);
      static void reshape (int width, int height);
      static void tick (void);
      static void output (int x, int y, const char *string);
      static void selectMessage (int msg);
      static void selectColor (int color);
      static void selectFont (int newfont);
      void display (Screen screen);
      static void disp (Menu menu);
      static void disp (Text text);
      //list<string> nodesToString (list<Node *> items);
      static void box (pair<float, float> pointA, pair<float, float> pointB);

      // Operations
    public:
      OpenGL (Presenter * presenter);
      ~OpenGL ();
      void init (int height, int width);
      void displayCursorPosition (int keybPosition);
  };

};

#endif
