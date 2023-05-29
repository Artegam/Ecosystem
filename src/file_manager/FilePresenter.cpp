#include "FileManager.h"

using namespace FileManager;

FilePresenter::FilePresenter () {
}

void FilePresenter::print (WorldModel data) {
}

list<string> FilePresenter::getSavedFiles () {
  list<string> files;
  string path = "/home/tonio/.ecosystem/";


  /* c++17
  for (const auto & entry : fs::directory_iterator(path))
    files.push_back(entry.path());
  */

  struct dirent *diread;
  dir_ptr = opendir(path.c_str());

  diread=readdir(dir_ptr);
  files.push_back(diread->d_name);

  do {
    diread = readdir(dir_ptr);
    if (diread != NULL) { // double protection obligatoire
      string str = diread->d_name;
      if (str.length() > 0)
        files.push_back(diread->d_name);
    }
  } while(diread != NULL);

  closedir(dir_ptr);

  files.sort();

  return files;
}
