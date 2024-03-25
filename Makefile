
## Repertoires par défaut dans lesquels make va chercher.
## L'ordre est important pour les recherches
VPATH = o:o/model/:o/screen_views:o/file_views:o/interactor:o/screen_listeners:o/screen_manager:o/file_manager:o/keyboards:o/sound:o/tests:src:includes

TESTS = tests/
INC = includes/
#INC_TU = $(SRC_TU)$(INC)
SRC = src/
OUT = o/
BIN = bin/
INSTALL_DIR = /usr/bin/
TEST_DIR = $(TESTS)$(SRC)

INCLUDES = -I $(INC) # -I $(INC_RENDER) -I $(TESTS)

LIBS = -lncurses -lstdc++fs -lopenal -lalut
#EXEC = Ecosystem
OPT = -Wall -g
OPT_THREAD = -std=c++0x -pthread


MODEL = $(wildcard src/model/*.cpp)
SCREEN_VIEWS = $(wildcard src/screen_views/*.cpp)
FILE_VIEWS = $(wildcard src/file_views/*.cpp)
INTERACTOR = $(wildcard src/interactor/*.cpp)
SCREEN_MANAGER = $(wildcard src/screen_manager/*.cpp)
FILE_MANAGER = $(wildcard src/file_manager/*.cpp)
KEYBOARDS = $(wildcard src/keyboards/*.cpp)
SOUND = $(wildcard src/sound/*.cpp)

MODEL_O=$(subst $(SRC), $(OUT), $(MODEL:.cpp=.o))
SCREEN_VIEWS_O=$(subst $(SRC), $(OUT), $(SCREEN_VIEWS:.cpp=.o))
FILE_VIEWS_O=$(subst $(SRC), $(OUT), $(FILE_VIEWS:.cpp=.o))
INTERACTOR_O=$(subst $(SRC), $(OUT), $(INTERACTOR:.cpp=.o))
SCREEN_MANAGER_O=$(subst $(SRC), $(OUT), $(SCREEN_MANAGER:.cpp=.o))
FILE_MANAGER_O=$(subst $(SRC), $(OUT), $(FILE_MANAGER:.cpp=.o))
KEYBOARDS_O=$(subst $(SRC), $(OUT), $(KEYBOARDS:.cpp=.o))
SOUND_O=$(subst $(SRC), $(OUT), $(SOUND:.cpp=.o))

OBJ=$(OUT)main.o
OBJ+=$(MODEL_O)
OBJ+=$(SCREEN_VIEWS_O)
OBJ+=$(FILE_VIEWS_O)
OBJ+=$(INTERACTOR_O)
OBJ+=$(SCREEN_MANAGER_O)
OBJ+=$(FILE_MANAGER_O)
OBJ+=$(KEYBOARDS_O)
OBJ+=$(SOUND_O)

#TESTS_U = test_unitaires
#O_TESTS_U = $(OBJECTS) test_unitaires.o TU_Loader.o TU_Moteur.o TU_MatParser.o

## Pour declarer des targets qui ne sont pas des fichiers
.PHONY: directories clean install install-libs install-cyg-libs uninstall uninstall-libs test

#all:$(EXEC)
all:Ecosystem test

Ecosystem: $(OBJ)
	g++ $(OPT) $(INCLUDES) $(OPT_THREAD) $(OBJ) -o $(BIN)$@ $(LIBS)

#$(TESTS_U): $(O_TESTS_U)
#	g++ $(OPT) $(INCLUDES) -o $(BIN)$@ o/*.o $(LIBS)

o/model/%.o: src/model/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@

o/screen_views/%.o: src/screen_views/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@

o/file_views/%.o: src/file_views/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@

o/interactor/%.o: src/interactor/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@

o/screen_manager/%.o: src/screen_manager/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@ -lstdc++fs

o/file_manager/%.o: src/file_manager/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@ -lstdc++fs

o/tests/%.o: src/tests/%.cpp
	g++ $(OPT) -c $(TESTS)$(INCLUDES) $^ -o $@

o/%.o: src/%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o $@

# A supprimer ?
#%.o: $(SRC_TU)%.cpp
#	g++ $(OPT) -c $(INCLUDES) $^ -o o/$@

directories:
	if [ ! -d o/ ]; then mkdir o/; fi
	if [ ! -d o/model ]; then mkdir o/model; fi
	if [ ! -d o/screen_views ]; then mkdir o/screen_views; fi
	if [ ! -d o/file_views ]; then mkdir o/file_views; fi
	if [ ! -d o/interactor ]; then mkdir o/interactor; fi
	if [ ! -d o/screen_manager ]; then mkdir o/screen_manager; fi
	if [ ! -d o/file_manager ]; then mkdir o/file_manager; fi
	if [ ! -d o/screen_listeners ]; then mkdir o/screen_listeners; fi
	if [ ! -d o/tests ]; then mkdir o/tests; fi
	if [ ! -d o/keyboards ]; then mkdir o/keyboards; fi
	if [ ! -d o/sound ]; then mkdir o/sound; fi
	if [ ! -d o/ ]; then mkdir o/; fi

clean:
	find o/ -name *.o | xargs rm;find . -name "*~" | xargs rm -f

install:
	sudo cp $(BIN)Ecosystem $(INSTALL_DIR)
	if [ ! -d ~/.ecosystem/ ]; then sudo mkdir ~/.ecosystem/; fi
	sudo chown tonio:tonio ~/.ecosystem/
	chmod 755 ~/.ecosystem/
	if [ ! -d /var/log/ecosystem/ ]; then sudo mkdir /var/log/ecosystem/; fi

install-tools: install-libs directories
	sudo apt-get install g++ vim

install-libs:
	sudo apt-get install libncurses5 libncursesw5-dev libncurses5-dev ncurses-doc libopenal-dev libopenal-data libopenal1 libalut-dev libaudio-dev

install-cyg-libs:
	apt-cyg install ncurses libncurses-devel

uninstall:
	sudo rm -f $(INSTALL_DIR)Ecosystem

uninstall-libs:
	sudo apt-get remove libncurses5 libncursesw5-dev libncurses5-dev ncurses-doc libopenal-dev libopenal-data libopenal1 libalut-dev libaudio-dev

uninstall-tools: uninstall-libs
	sudo apt-get remove g++ vim
	rm -r o/

test:
	#TODO: Penser a un repertoire de test dedie aux tests
    #cli commands here for test predicate
	# compile tests classes
	# run test classes
