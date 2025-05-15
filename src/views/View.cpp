#include "Views.h"


using namespace std;
using namespace Views;

View::View (Presenter * presenter) {
  _presenter = presenter;
}

void View::init (int height, int width) {
}

void View::end () {
  tobeClosed = true;
}

bool View::shouldClose () {
  return tobeClosed;
}

void View::close () {
}

void View::display () {
}

void View::display (Screen screen) {
  _maxKeyboardx = screen.getSelectSize() - 1;
}

void View::display (Menu menu) {
}

void View::display (Text text) {
}

void View::mainMenu (int keybPosition) {
}

void View::options (int keybPosition) {
}

void View::languages (int keybPosition) {
}

void View::video (int keybPosition) {
}

void View::gameplay (int position, map<int, int> worldMap) {
}

int View::getChar () {
  return 0;
}

void View::initScreen () {
}

void View::clearScreen () {
}

void View::validateOption (int optionNumber) {
}

void View::createWindow (int position, int x, int y, int height, int width) {
}

void View::keyboard () {
}

const int View::getKeyboardx () {
  return _keyboardx;
}

const int View::getKeyboardy () {
  return _keyboardy;
}

void View::setMaxKeyboardx (const int max) {
  _maxKeyboardx = max;
}

void View::setMaxKeyboardy (const int max) {
  _maxKeyboardy = max;
}

void View::resetKeyboard () {
  _keyboardx = 0;
  _keyboardy = 0;
}

bool View::isValid () {
  bool ret = _valid;
  _valid = false;
  return ret;
}

void View::setLanguage (const unsigned int lang) {
  dict.setLanguage(lang);
}

size_win View::getScreenSize () {
  return screenSize;
}
