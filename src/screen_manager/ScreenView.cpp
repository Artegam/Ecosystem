#include "ScreenManager.h"

using namespace ScreenManager;


ScreenView::ScreenView () {
}


void ScreenView::init (int worldHeight, int worldWidth) {
}
/*
void ScreenView::display (ScreenViewModel * data) {
}
*/
void ScreenView::mainMenu () {
}

void ScreenView::loadMenu (list<string> files) {
}

void ScreenView::infos (list<string> infos) {
}

void ScreenView::gameplay (map<int, int> worldMap, list<Wildlife *> wildlifes) {
}

void ScreenView::end () {
}

void ScreenView::keyboardListener(WorldModel worldData) {
}

int ScreenView::getScreen () {
  return 0;
}
