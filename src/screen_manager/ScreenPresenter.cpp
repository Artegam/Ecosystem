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

    //Keyboard management here...
    keyb->listen(this->view->getChar());

    //TODO: Essayer de supprimer la gestion de changement d ecran avec la variable screen
    // voir comment on peux faire avec la liste des nodes
    switch (screen) {
      default: //MAIN menu
        if(keyb->isValid()) {
          switch (keyb->getPosition()) {
            case 0:
              changeScreen(IN_GAME);
              break;
            case 1:
              changeScreen(OPTIONS);
              data->validate(1);
              keyb->setPositionsCount((int)data->getMenu().size());
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
        } else {
          keyb->setPositionsCount((int)menuSize);
          this->view->mainMenu(keyb->getPosition());
        }
        keyb->resetValid();
        break;

      case OPTIONS:
        if(keyb->isValid()) {
          switch (keyb->getPosition()) {
            case 0:
              changeScreen(LANGUAGES);
              data->validate(0);
              break;
            case 1:
              changeScreen(VIDEO);
              data->validate(1);
              break;
            case 3: // Back
              changeScreen(MAIN);
              data->back();
              break;
          }
          keyb->setPositionsCount((int)data->getMenu().size());
        } else {
          this->view->options(keyb->getPosition()); // Affiche l ecran options grace a la vue
        }
        keyb->resetValid();
        break;

      case LANGUAGES:
        if(keyb->isValid()) {
          switch (keyb->getPosition()) {
            case 0:
              data->setLanguage(0);
              this->view->validateOption(0);
              this->view->clearScreen();
              break;
            case 1:
              data->setLanguage(1);
              this->view->validateOption(1);
              this->view->clearScreen();
              break;
            case 3: // Back
              changeScreen(OPTIONS);
              data->back();
              keyb->setPositionsCount((int)data->getMenu().size());
              break;
          }
        } else {
          this->view->languages(keyb->getPosition()); // Affiche l ecran languages grace a la vue
        }
        keyb->resetValid();
        break;

      case VIDEO:
        if(keyb->isValid()) {
          switch (keyb->getPosition()) {
            case 0:
              this->view->validateOption(0);
              break;
            case 1:
              this->view->validateOption(1);
              break;
            case 3: // Back
              changeScreen(OPTIONS);
              data->back();
              keyb->setPositionsCount((int)data->getMenu().size());
              break;
          }
        } else {
          this->view->video(keyb->getPosition()); // Affiche l ecran video grace a la vue
        }

        keyb->resetValid();
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
            if ((unsigned int)keyb->getPosition() == (size - 1))
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
        {
          // TODO: Recuperer la liste des fichiers ici et la passer a load
          list<string> files = fm->getSavedFiles();
          unsigned int size = 2;
          if(files.size() > 0)
            size = files.size() + 1;

          if(keyb->isValid()) {
            unsigned int choice = 0;
            if ((unsigned int)keyb->getPosition() == (size - 1))
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
            this->view->save(files, size, keyb->getPosition());
            keyb->resetValid();
          }
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

