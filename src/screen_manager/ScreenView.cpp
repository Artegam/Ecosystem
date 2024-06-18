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
void ScreenView::mainMenu (int keybPosition) {
}

void ScreenView::options (list<string> options, int keybPosition) {
}

void ScreenView::load (list<string> files, const unsigned int menuSize, int keybPosition) {
}

void ScreenView::save (list<string> files, const unsigned int menuSize, int keybPosition) {
}

/*
void ScreenView::save (string filename) {
}
*/

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

void ScreenView::clearOptions (list<Node *> options) {
	list<Node *>::iterator option;

  for(option = options.begin(); option != options.end(); option++) {
		(*option)->clear();
	}
}

void ScreenView::validateOption (int optionNumber) {
}
