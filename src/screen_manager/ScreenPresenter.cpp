#include "ScreenManager.h"

using namespace ScreenManager;

ScreenManager::ScreenPresenter::ScreenPresenter (ScreenView * view, World * world) {
  this->world = world;
  this->data = new ScreenViewModel(world->getData());
  this->view = view;
}

void ScreenManager::ScreenPresenter::setStatus (const int newStatus) {
  status = newStatus;
}

void ScreenManager::ScreenPresenter::display () {
  // TODO: C'est ici que l'on gere l'affichage des menu et le comportement de l'affichage global
  // Est-ce qu'il faut ajouter une ou plusieur variable de status ???
  this->world->run();
  //status = GAMEPLAY;
  this->view->init(world->getData().getHeight(), world->getData().getWidth());
  while(1) {
    status = this->view->getScreen();
    if(status == data->MAIN_MENU) {
      this->view->mainMenu();
    } else if (status == data->LOAD_MENU) {
      // TODO: Recuperer la liste des fichiers ici et la passer a loadMenu
      FilePresenter * fm = new FilePresenter();
      this->view->loadMenu(fm->getSavedFiles());
    } else if (status == data->IN_GAME) {
      this->view->infos(getInfos());
      this->view->gameplay(data->getWorldData().getWorldMap(), data->getWildlife());
      this->view->keyboardListener(world->getData()); //TODO: A mettre a jour avec keyboard::NCurses
    } else if (status == data->GAME_OVER) {
      this->view->end();
    } else { // cas par defaut
      this->view->mainMenu();
    }
  }
}

list<string> ScreenManager::ScreenPresenter::getInfos () {
  list<string> lst;
  if(data->getTic()) {
    lst.push_back("tic");
  } else {
    lst.push_back("tac");
  }
  lst.push_back("Living beings number:" + to_string(data->getWildlifeCount()));
  if(data->isRunning()) {
    lst.push_back("execution: running...");
  } else {
    lst.push_back("execution: paused");
  }
  lst.push_back("turns: " + to_string(data->getTurns()));
  lst.push_back("age average: " + to_string(data->getAverageAge()));
  lst.push_back("life expectancy: " + to_string(data->getLifeExpectancy()));
  return lst;
}

// Ici ce n est que pour les ecrans
void ScreenManager::ScreenPresenter::start () {
}

void ScreenManager::ScreenPresenter::load () {
}

void ScreenManager::ScreenPresenter::save () {
}

void ScreenManager::ScreenPresenter::end () {
}

ScreenViewModel * ScreenManager::ScreenPresenter::getData () {
  return this->data;
}



void ScreenManager::ScreenPresenter::print (WorldModel data) {
}

