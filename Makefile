
## Repertoires par défaut dans lesquels make va chercher.
## L'ordre est important pour les recherches
VPATH = o:o/model/:o/views:o/logs:o/file_views:o/interactor:o/screen_listeners:o/screen_manager:o/file_manager:o/keyboards:o/sound:o/tests:src:includes

CXX := g++
TESTS = tests/
INC = includes/
#INC_TU = $(SRC_TU)$(INC)
SRC = src/
OUT = o/
BIN = bin/
INSTALL_DIR = /usr/bin/
TEST_DIR = $(TESTS)$(SRC)

INCLUDES = -I $(INC) -I/usr/include/freetype2 -I/usr/include/libpng16 # -I $(INC_RENDER) -I $(TESTS)

LIBS = -lncurses -lGL -lSDL2 -lSDL2_image -lSDL2_ttf -lstdc++fs -lopenal -lalut -L/usr/lib64 -lfreetype -lglfw -lGLESv2 -lm
OPT = -Wall -g
OPT_THREAD = -std=c++0x -pthread


SRC_FILES = $(shell find src/ -type f -name '*.cpp')
OBJ_FILES = $(patsubst src/%.cpp, o/%.o, $(SRC_FILES))
#DEP_FILES = $(patsubst src/%.cpp, d/%.d, $(SRC_FILES))

#TESTS_U = test_unitaires
#O_TESTS_U = $(OBJECTS) test_unitaires.o TU_Loader.o TU_Moteur.o TU_MatParser.o

## Pour declarer des targets qui ne sont pas des fichiers
.PHONY: clean install install-libs install-cyg-libs uninstall uninstall-libs test

all: Ecosystem test

Ecosystem: $(OBJ_FILES)
	g++ $(OPT) $(INCLUDES) $(OPT_THREAD) $^ -o $(BIN)$@ $(LIBS)

#$(TESTS_U): $(O_TESTS_U)
#	g++ $(OPT) $(INCLUDES) -o $(BIN)$@ o/*.o $(LIBS)

# ASC - 15/05/2025 - pour le calcul des dépendances
#d/%.d: src/%.cpp
#	@mkdir -p "$(@D)"
#	$(CXX) $(INCLUDES) -M -MMD $< -o $@
#
o/%.o: src/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(OPT) -c $(INCLUDES) $? -o $@

# A supprimer ?
#%.o: $(SRC_TU)%.cpp
#	g++ $(OPT) -c $(INCLUDES) $^ -o o/$@

clean:
	rm -r o/;find . -name "*~" | xargs rm -f

install:
	sudo cp $(BIN)Ecosystem $(INSTALL_DIR)
	if [ ! -d ~/.ecosystem/ ]; then sudo mkdir ~/.ecosystem/; fi
	sudo chown tonio:tonio ~/.ecosystem/
	chmod 755 ~/.ecosystem/
	if [ ! -d /var/log/ecosystem/ ]; then sudo mkdir /var/log/ecosystem/; fi

install-tools: install-libs
	sudo apt-get install g++ vim dia dia2code doxygen

install-ncurses:
	sudo apt-get install libncurses-dev ncurses-doc

install-opengl:
	sudo apt-get install libgl-dev libsdl2-dev libsdl2-ttf-dev libglfw3-dev libglfw3-doc libglfw3 libsdl2-image-2.0-0 libsdl2-image-dev

install-libs: install-ncurses install-opengl
	sudo apt-get install libopenal-dev libopenal-data libopenal1 libalut-dev libaudio-dev libfreetype6-dev freetype2-doc libglm-dev libglm-doc

install-cyg-libs:
	apt-cyg install ncurses libncurses-devel

uninstall:
	sudo rm -f $(INSTALL_DIR)Ecosystem

uninstall-tools: uninstall-libs
	sudo apt-get remove g++ vim dia dia2code doxygen
	rm -r o/

uninstall-ncurses:
	sudo apt-get remove libncurses-dev ncurses-doc

uninstall-opengl:
	sudo apt-get remove libgl-dev libsdl2-dev libsdl2-ttf-dev libglfw3-dev libglfw3-doc libglfw3 libsdl2-image-2.0-0 libsdl2-image-dev

uninstall-libs: uninstall-ncurses uninstall-opengl
	sudo apt-get remove libopenal-dev libopenal-data libopenal1 libalut-dev libaudio-dev libfreetype6-dev freetype2-doc libglm-dev libglm-doc

test:
	#TODO: Penser a un repertoire de test dedie aux tests
#cli commands here for test predicate
	# compile tests classes
	# run test classes

include $(DEP_FILES)
