#include "FileViews.h"

using namespace FileViews;

Text::Text (const char * filepath) {
  this->filepath = filepath;
}

void Text::init (void) {
}

void Text::save (void) {
  WorldModel worldData;
  worldData = data->getWorldData();
  // Attention avec les droits des repertoires
  const char* path = "/home/tonio/.ecosystem/save1.dat";
  ofstream f(path);
  if (f.is_open()) {
    string str;
    f << worldData.getHeight() << endl;
    f << worldData.getWidth() << endl;
    //Clock
    f << worldData.getClock()->isRunning() << endl;
    f << worldData.getClock()->getTurns() << endl;
    f << worldData.getClock()->getT() << endl;
    f << worldData.getClock()->interval << endl;
    // Clock subscribers
    map<string, ClockSubscriber *> subscribers = worldData.getClock()->getSubscribers();
    map<string, ClockSubscriber *>::iterator it;
    for(it = subscribers.begin(); it != subscribers.end(); it++) {
      Wildlife * wl = (Wildlife *)it->second;
      GenericModel * data = wl->getData();
      str = data->getRawData();
      f << str << endl;
    }
    //World map
    f << worldData.getRawData() << endl;
    // Screen View Model
    f << data->getRawData() << endl;
    f.flush();
    f.close();
  }
}

void Text::load(void) {
  WorldModel worldData;
  const char* path = "/home/tonio/.ecosystem/save1.dat";
  ifstream f(path);
  if (f.is_open()) {
    string str;
    getline(f, str);
    worldData.setHeight(stoi(str));
    getline(f, str);
    worldData.setWidth(stoi(str));
    //Clock
/*
    f << worldData.getHeight() << endl;
    f << worldData.getWidth() << endl;
    //Clock
    f << worldData.getClock()->isRunning() << endl;
    f << worldData.getClock()->getTurns() << endl;
    f << worldData.getClock()->getT() << endl;
    f << worldData.getClock()->interval << endl;
    // Clock subscribers
    map<string, ClockSubscriber *> subscribers = worldData.getClock()->getSubscribers();
    map<string, ClockSubscriber *>::iterator it;
    for(it = subscribers.begin(); it != subscribers.end(); it++) {
      Wildlife * wl = (Wildlife *)it->second;
      GenericModel * data = wl->getData();
      str = data->getRawData();
      f << str << endl;
    }
    //World map
    f << worldData.getRawData() << endl;
    // Screen View Model
    f << getRawData() << endl;
    f.flush();
    */
    f.close();
  }
}

