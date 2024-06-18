#include "ScreenManager.h"

using namespace ScreenManager;

ScreenPresenter::ScreenPresenter (World * world, ScreenViewModel * svm, Keyboard * kb, ScreenView * sv) {
  this->world = world;
  this->data = svm;
  this->keyb = kb;
  this->view = sv;
}

void ScreenPresenter::setStatus (const int newStatus) {
  screen = newStatus;
}

void ScreenPresenter::display () {
  const unsigned int menuSize = 5;
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
          keyb->setPositionsCount((int)menuSize);
          this->view->mainMenu(keyb->getPosition()); //TODO: Traiter aussi le menu principal comme pour les options
          keyb->resetValid();
        }
        break;

      case OPTIONS:
        // On a une position ici ???
        if(keyb->isValid()) {
          switch (keyb->getPosition()) {
            case 0:
              this->view->validateOption(0);
              break;
            case 1:
              this->view->validateOption(1);
              break;
            case 3: // Back
              changeScreen(MAIN);
              break;
          }
          keyb->resetValid();
        } else {
          Node * root = this->data->getMenu();
          list<Node *> menu =  root->getChildren();
          list<Node *>::iterator it = menu.begin();
          advance(it, 1);
          list<Node *> opts =  (*it)->getChildren();
          keyb->setPositionsCount((int)opts.size());
          this->view->options(lst_options, keyb->getPosition());
          keyb->resetValid();
        }
        break;

      case LOAD:
        {
          // TODO: Recuperer la liste des fichiers ici et la passer a load
          list<string> files = fm->getSavedFiles();
          unsigned int size = 2;
          if(files.size() > 0)
            size = files.size() + 1;

          if(keyb->isValid()) {
            unsigned int choice = 0;
            if (keyb->getPosition() == (size - 1))
              choice = 2;

            switch (choice) {
              case 0:
                // TODO: Select file to be loaded
                //changeScreen(IN_GAME);
                break;
              case 2: // Back to main menu
                changeScreen(MAIN);
                break;
            }
            keyb->resetValid();
          } else {
            keyb->setPositionsCount((int)size);
            this->view->load(files, size, keyb->getPosition());
            keyb->resetValid();
          }
        }
        break;

      case SAVE:
        // TODO: Recuperer la liste des fichiers ici et la passer a load
        if(keyb->isValid()) {
          switch (keyb->getPosition()) {
            case 0:
              //changeScreen(IN_GAME);
              // Select first option
              break;
            case 1: // Back
              changeScreen(MAIN);
              break;
          }
          keyb->resetValid();
        } else {
          //TODO: Bug here !!!
          this->view->save("save.sav");
        }
        break;

      case IN_GAME:
        this->view->infos(getInfos());
        this->view->gameplay();
        //TODO: ICI gestion clavier pour la partie en cours ????
        this->view->keyboardListener(world->getData()); //TODO: A mettre a jour avec keyboard::NCurses
        keyb->resetValid();
        break;

      case GAME_OVER:
        if(keyb->isValid()) {
          keyb->resetValid();
          exit(0);
        } else {
        this->view->end();
        }
        break;

      default:
        keyb->setPositionsCount((int)menuSize);
        this->view->mainMenu(keyb->getPosition());
        keyb->resetValid();
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
  keyb->defaultPosition();
  screen = nextScreen;
  this->view->clearScreen();
}


