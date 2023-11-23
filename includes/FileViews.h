#include "FileManager.h"
#include <ncurses.h>
#include <unistd.h>

using namespace FileManager;

namespace FileViews {

  /// class NCursesView -
  class Text : public FileView {
    // Attributes
    FileViewModel * data;
    string filepath = "";
    // Operations
    public:
    Text ();
    Text (const char * filepath);
    ~Text ();
    void init ();
    void save ();
    void load ();
  };
}

