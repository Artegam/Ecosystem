#include "Screen.h"

using namespace Screen;

ScreenPresenter::ScreenPresenter (ScreenView * view, World * world) {
  this->world = world;
  this->data = new ScreenViewModel(world->getData());
  this->view = view;
}


void ScreenPresenter::display () {
  this->world->run();
  while(1) {
    this->view->display(this->data);
  }
}

