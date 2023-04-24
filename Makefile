

## Repertoires par défaut dans lesquels make va chercher.
## L'ordre est important pour les recherches
VPATH = o:o/controller:o/interactor:o/screen_presenter:src:includes

SRC_TU = tests/

INC = includes/
#INC_TU = $(SRC_TU)$(INC)
OUT = o/
BIN = bin/

INCLUDES = -I $(INC) # -I $(INC_RENDER) -I $(INC_TU)

INSTALL_DIR = /usr/bin/

LIBS = -lncurses
EXEC = Ecosystem
OPT = -Wall -g
OPT_THREAD = -std=c++0x -pthread


MODEL = $(wildcard src/model/*.cpp)
VIEWS = $(wildcard src/views/*.cpp)
CONTROLLER = $(wildcard src/controller/*.cpp) #ClockSubscriber.o Clock.o World.o
#INTERACTOR = Explorer.o Survival.o Aggressive.o WildlifeModel.o Behavior.o Fish.o Shark.o Wildlife.o
INTERACTOR = $(wildcard src/interactor/*.cpp) #Explorer.o WildlifeModel.o Behavior.o Fish.o Shark.o Wildlife.o
SCREEN_PRESENTER = $(wildcard src/screen_presenter/*.cpp) #ScreenViewModel.o ScreenView.o ScreenPresenter.o

MODEL_O=$(subst src/, o/, $(MODEL:.cpp=.o))
VIEWS_O=$(subst src/, o/, $(VIEWS:.cpp=.o))
CONTROLLER_O=$(subst src/, o/, $(CONTROLLER:.cpp=.o))
INTERACTOR_O=$(subst src/, o/, $(INTERACTOR:.cpp=.o))
SCREEN_PRESENTER_O=$(subst src/, o/, $(SCREEN_PRESENTER:.cpp=.o))

OBJ=o/main.o
OBJ+=$(MODEL_O)
OBJ+=$(VIEWS_O)
OBJ+=$(CONTROLLER_O)
OBJ+=$(INTERACTOR_O)
OBJ+=$(SCREEN_PRESENTER_O)

#TESTS_U = test_unitaires
#O_TESTS_U = $(OBJECTS) test_unitaires.o TU_Loader.o TU_Moteur.o TU_MatParser.o

## Pour declarer des targets qui ne sont pas des fichiers
.PHONY: clean mrproper install install-libs uninstall uninstall-libs

#all:$(EXEC) $(TESTS_U)
all:$(EXEC)


$(EXEC): $(OBJ)
	g++ $(OPT) $(INCLUDES) $(OPT_THREAD) $(OBJ) -o $(BIN)$@ $(LIBS)

$(TESTS_U): $(O_TESTS_U)
	g++ $(OPT) $(INCLUDES) -o $(BIN)$@ o/*.o $(LIBS)

o/model/%.o: src/model/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@

o/views/%.o: src/views/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@

o/controller/%.o: src/controller/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@

o/interactor/%.o: src/interactor/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@

o/screen_presenter/%.o: src/screen_presenter/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@

o/%.o: src/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@

%.o: $(SRC_TU)%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o o/$@

directories:
	if [ ! -d o/model ]; then mkdir o/model; fi
	if [ ! -d o/views ]; then mkdir o/views; fi
	if [ ! -d o/controller ]; then mkdir o/controller; fi
	if [ ! -d o/interactor ]; then mkdir o/interactor; fi
	if [ ! -d o/screen_presenter ]; then mkdir o/screen_presenter; fi
	if [ ! -d o/ ]; then mkdir o/; fi

clean:
	find o/ -name *.o | xargs rm;find . -name "*~" | xargs rm -f

mrproper: clean
	rm -rf $(EXEC)

install:
	cp $(EXEC) $(INSTALL_DIR)

uninstall:
	rm $(INSTALL_DIR)$(EXEC)

install-libs:
	sudo apt-get install libncurses5 libncursesw5-dev libncurses5-dev ncurses-doc

install-cyg-libs:
	apt-cyg install ncurses libncurses-devel

uninstall-libs:
	sudo apt-get remove libncurses-dev
