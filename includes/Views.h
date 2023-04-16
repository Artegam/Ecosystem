#include "Screen.h"
#include <ncurses.h>
#include <unistd.h>

using namespace Screen;

namespace Views {

  /// class NCursesView -
  class NCursesView : public ScreenView {
    // Attributes
    ScreenViewModel * data;
    // Operations
  public:
    NCursesView ();
    ~NCursesView ();
    void init ();
    void menu (WINDOW * main);
    void display (ScreenViewModel * data);
  };

};

