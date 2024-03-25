#include "ScreenManager.h"

using namespace ScreenManager;


ScreenView::ScreenView () {
}


void ScreenView::init (ScreenViewModel * data) {
  model = data;
}
/*
void ScreenView::display (ScreenViewModel * data) {
}
*/
void ScreenView::mainMenu () {
}

void ScreenView::options (list<string> options) {
}

void ScreenView::load (list<string> files) {
}

void ScreenView::infos (list<string> infos) {
}

void ScreenView::gameplay () {
}

void ScreenView::end () {
}

void ScreenView::keyboardListener(WorldModel worldData) {
}

int ScreenView::getChar () {
  return 0;
}

void ScreenView::clearScreen () {
  toClear = true;
}
