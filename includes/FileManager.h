#include "Interactor.h"
#include <list>
//#include <filesystem> // pour windows seulement ?? compilation c++17 ???
#include <dirent.h>

using namespace Interactor;
//namespace fs = std::filesystem;

namespace FileManager {

  // DataStructure
  /// class FileViewModel - 
  class FileViewModel : public Loggable, public GenericModel {
    // Attributes
		WorldModel worldData;
    list<string> messages = Loggable::log();
  public:
    FileViewModel (WorldModel worldData);
    int getWorldHeight ();
    int getWorldWidth ();
		WorldModel getWorldData ();
    bool isRunning ();
    bool getTic ();
    unsigned int getWildlifeCount ();
    list<Wildlife *> getWildlife ();
    bool isWildlife (const ClockSubscriber * ptr);
    unsigned int getTurns ();
    const unsigned int getAverageAge();
    const unsigned int getLifeExpectancy ();
    string getRawData ();
    list<string> log();
  };

  // interface
  /// class FileView - 
  class FileView {
    // Operations
  public:
    FileView ();
    FileView (const char * filepath);
    virtual void save ();
    virtual void load ();
  };

  /// class FilePresenter - 
  class FilePresenter : public OutputDevice {
    // Attributes
  private:
    FileView * view;
    FileViewModel * data;
    World * world;
    DIR *dir_ptr;
    struct dirent *diread;
    // Operations
  public:
    FilePresenter ();
    void print (WorldModel data);
    list<string> getSavedFiles ();
  };

};

