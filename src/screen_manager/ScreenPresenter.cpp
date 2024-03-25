#include "ScreenManager.h"

using namespace ScreenManager;

ScreenPresenter::ScreenPresenter (World * world, ScreenViewModel * svm, Keyboard * kb, ScreenView * sv) {
  this->world = world;
  this->data = svm;
  this->keyb = kb;
  this->view = sv;
  /*
     this->data = new ScreenViewModel(world->getData());
     this->keyb = new Keyboards::NCurses();
     this->view = new ScreenViews::NCurses(keyb);
     */
}

void ScreenPresenter::setStatus (const int newStatus) {
  screen = newStatus;
}

void ScreenPresenter::display () {
  list<string> lst_options;
  FilePresenter * fm = new FilePresenter();

  // TODO: C'est ici que l'on gere l'affichage des menu et le comportement de l'affichage global
  // Avoir l'ecran en cours d'affichage pour traiter les options possibles
  this->world->run();
  this->view->init(data);
  while(1) {

    // TODO: Keyboard management here...
    keyb->listen(this->view->getChar());
    //int position = keyb->getPosition();
 /*else if (keyb->isValid() && position == (const unsigned int)(size-1)) {
      changeScreen(MAIN);
    }*/

    switch (screen) {
      case MAIN:
        if(keyb->isValid()) {
          switch (keyb->getPosition()) {
            case 0:
              changeScreen(IN_GAME);
              break;
            case 1:
              changeScreen(OPTIONS);
              break;
            case 2:
              changeScreen(SAVE);
              break;
            case 3:
              changeScreen(LOAD);
              break;
            case 4:
              changeScreen(GAME_OVER);
              break;
          }
          keyb->resetValid();
        } else {
          this->view->mainMenu(); //TODO: Traiter aussi le menu principal comme pour les options
        }
        break;

      case OPTIONS:
        // On a une position ici ???
        if(keyb->isValid()) {
          switch (keyb->getPosition()) {
            case 0:
              //changeScreen(IN_GAME);
              // Select first option
              break;
            case 1:
              //changeScreen(OPTIONS);
              // Select second option
              break;
            case 3: // Back
              changeScreen(MAIN);
              break;
          }
          keyb->resetValid();
        } else {
          this->view->options(lst_options);
        }
        break;

      case LOAD:
        // TODO: Recuperer la liste des fichiers ici et la passer a load
        this->view->load(fm->getSavedFiles());
        break;

      case IN_GAME:
        this->view->infos(getInfos());
        this->view->gameplay();
        //TODO: ICI gestion clavier pour la partie en cours ????
        this->view->keyboardListener(world->getData()); //TODO: A mettre a jour avec keyboard::NCurses
        break;

      case GAME_OVER:
        this->view->end();
        break;

      default:
        this->view->mainMenu();
        break;
    }
  }
}

list<string> ScreenPresenter::getInfos () {
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
void ScreenPresenter::start () {
}

void ScreenPresenter::load () {
}

void ScreenPresenter::save () {
}

void ScreenPresenter::end () {
}

ScreenViewModel * ScreenPresenter::getData () {
  return this->data;
}



void ScreenPresenter::print (WorldModel data) {
}

int ScreenPresenter::getScreen () {
  return screen;
}

void ScreenPresenter::changeScreen (const int nextScreen) {
  screen = nextScreen;
  this->view->clearScreen();
}


