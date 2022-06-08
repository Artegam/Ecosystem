#include "Screen.h"

using namespace Screen;

ScreenPresenter::ScreenPresenter (ScreenView * view, World * world) {
  this->world = world;
  this->data = new ScreenViewModel(world);
  this->view = view;
}


void ScreenPresenter::display () {
  this->world->run();
  while(this->data->isRunning()) {
    this->view->display(this->data);
  }
}

