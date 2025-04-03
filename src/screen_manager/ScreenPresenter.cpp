#include "ScreenManager.h"

using namespace ScreenManager;

ScreenPresenter::ScreenPresenter (World * world, ScreenViewModel svm) {
  this->world = world;
  this->data = svm;
}

void ScreenPresenter::start () {
  //view = new NCurses(this);
  view = new OpenGL(this); //ERROR: La boucle infinie de opengl neutralise le presenter
  view->createWindow(MAIN, 0, 0, 50, 50);

  view->init(data.getWorldHeight(), data.getWorldWidth());
  //view->display(data.getScreen(MAIN));
  //fct = (void *) display();
  //***************************************************

  view->display(data.getScreen(MAIN));
  view->setMaxKeyboardx((int)data.getMenu().size());
  display();
}

void ScreenPresenter::setStatus (const int newStatus) {
  screen = newStatus;
}

void ScreenPresenter::display () {
  list<string> lst_options;
  //FilePresenter * fm = new FilePresenter();
  Screen s = data.getScreen(MAIN);

  while (!view->shouldClose())
  {
    view->display();
    // C'est ici que l'on gere l'affichage des menu et le comportement de l'affichage global
    // Avoir l'ecran en cours d'affichage pour traiter les options possibles
    this->world->run();
    //TODO: Essayer de supprimer la gestion de changement d ecran avec la variable screen
    switch (screen) {
      default: //MAIN menu
        if(view->isValid()) {
          switch (view->getKeyboardx()) {
            case 0:
              changeScreen(IN_GAME);
              break;
            case 1:
              data.validate(1);
              view->createWindow(OPTIONS, 98, 20, 6, 11);
              changeScreen(OPTIONS);
              break;
            case 2:
              data.validate(2);
              this->view->createWindow(SAVE, 98, 20, 6, 13);
              changeScreen(SAVE);
              s = data.getScreen(SAVE);
              s.resize(data.getMenu().size(), computeMaxWidth(data.getMenuText()));
              break;
            case 3:
              data.validate(3);
              this->view->createWindow(LOAD, 98, 20, 6, 13);
              changeScreen(LOAD);
              s = data.getScreen(LOAD);
              s.resize(data.getMenu().size(), computeMaxWidth(data.getMenuText()));
              break;
            case 4:
              changeScreen(GAME_OVER);
              break;
          }
        } else {
          this->view->display(data.getScreen(MAIN));
        }
        break;

      case OPTIONS:
        if(this->view->isValid()) {
          switch (this->view->getKeyboardx()) {
            case 0:
              this->view->createWindow(LANGUAGES, 99, 21, 6, 13);
              changeScreen(LANGUAGES);
              data.validate(0);
              break;
            case 1:
              this->view->createWindow(VIDEO, 99, 21, 6, 13);
              changeScreen(VIDEO);
              data.validate(1);
              break;
            case 3: // Back
              changeScreen(MAIN);
              data.back();
              break;
          }
        } else {
          this->view->display(data.getScreen(OPTIONS));
        }
        break;

      case LANGUAGES:
        if(this->view->isValid()) {
          switch (this->view->getKeyboardx()) {
            case 0:
              view->setLanguage(0);
              view->validateOption(0);
              view->clearScreen();
              break;
            case 1:
              view->setLanguage(1);
              view->validateOption(1);
              view->clearScreen();
              break;
            case 3: // Back
              changeScreen(OPTIONS);
              data.back();
              break;
          }
        } else {
          this->view->display(data.getScreen(LANGUAGES));
        }
        break;

      case VIDEO:
        if(this->view->isValid()) {
          switch (this->view->getKeyboardx()) {
            case 0:
              view->validateOption(0);
              view->clearScreen();
              data.setMode(1);
              view = new NCurses(this);
              break;
            case 1:
              view->validateOption(1);
              view->clearScreen();
              data.setMode(2);
              view = new OpenGL(this); //ERROR: La boucle infinie de opengl neutralise le presenter
              break;
            case 3: // Back
              changeScreen(OPTIONS);
              data.back();
              break;
          }
        } else {
          this->view->display(data.getScreen(VIDEO));
        }
        break;

      case LOAD:
        {
          // TODO: Recuperer la liste des fichiers ici et la passer a load
          /*
             list<string> files = fm->getSavedFiles();
             unsigned int size = 2;
             if(files.size() > 0)
             size = files.size() + 1;
           */
          if(this->view->isValid()) {
            switch (this->view->getKeyboardx()) {
              case 0:
                // TODO: Select file to be loaded
                //changeScreen(IN_GAME);
                break;
              case 2: // Back to main menu
                changeScreen(MAIN);
                data.back();
                break;
            }
          } else {
            this->view->display(data.getScreen(LOAD));
          }
        }
        break;

      case SAVE:
        {
          // TODO: Recuperer la liste des fichiers ici et la passer a load
          /*
             list<string> files = fm->getSavedFiles();
             unsigned int size = 2;
             if(files.size() > 0)
             size = files.size() + 1;
           */
          if(this->view->isValid()) {
            //TODO: if ((unsigned int)keyb->getPosition() == (size - 1))
            switch (this->view->getKeyboardx()) {
              case 0:
                // TODO: Select file to be overwrited
                //changeScreen(IN_GAME);
                break;
              case 2: // Back to main menu
                changeScreen(MAIN);
                data.back();
                break;
            }
            //keyb->resetValid();
          } else {
            this->view->display(data.getScreen(SAVE));
          }
        }
        break;

      case IN_GAME:
        //TODO: a remplacer par display(rawData);
        //this->view->infos(getInfos());
        this->view->gameplay(IN_GAME, data.getWorldData().getWorldMap());
        //TODO: revoir la gestion du clavier, c'est un input pas un output
        //this->view->keyboardListener(world->getData()); //TODO: A mettre a jour avec keyboard::NCurses
        //keyb->resetValid();
        break;

      case GAME_OVER:
        if(view->isValid()) {
          exit(0);
        } else {
          view->end();
        }
        break;
    }
  }
  view->close();
}

list<string> ScreenPresenter::getInfos () {
  list<string> lst;
  if(data.getTic()) {
    lst.push_back("tic");
  } else {
    lst.push_back("tac");
  }
  lst.push_back("Living beings number:" + to_string(data.getWildlifeCount()));
  if(data.isRunning()) {
    lst.push_back("execution: running...");
  } else {
    lst.push_back("execution: paused");
  }
  lst.push_back("turns: " + to_string(data.getTurns()));
  lst.push_back("age average: " + to_string(data.getAverageAge()));
  lst.push_back("life expectancy: " + to_string(data.getLifeExpectancy()));
  return lst;
}

// Ici ce n est que pour les ecrans
/*
void ScreenPresenter::start () {
}
*/
void ScreenPresenter::load () {
}

void ScreenPresenter::save () {
}

void ScreenPresenter::end () {
}

ScreenViewModel ScreenPresenter::getData () {
  return this->data;
}

void ScreenPresenter::print (WorldModel data) {
}

int ScreenPresenter::getScreen () {
  return screen;
}

void ScreenPresenter::changeScreen (const int nextScreen) {
  //keyb->defaultPosition();
  screen = nextScreen;
  view->resetKeyboard();
  view->clearScreen();
}

const unsigned int ScreenPresenter::computeMaxWidth (list<string> menu) {
  list<string>::iterator it;
  unsigned int max = 0;
  unsigned int len = 0;

  for(it = menu.begin(); it != menu.end(); it++) {
    len = (*it).size();
    if (max < len)
      max = len;
  }

  return max;
}



