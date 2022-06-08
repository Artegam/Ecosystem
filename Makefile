

VPATH = src

SRC_TU = tests/

INC = includes/

#INC_TU = $(SRC_TU)$(INC)

OUT = o/
BIN = bin/

INCLUDES = -g -I $(INC) # -I $(INC_RENDER) -I $(INC_TU)

INSTALL_DIR = /usr/bin/

LIBS = -lncurses
EXEC = Ecosystem
OPT = -Wall
OPT_THREAD = -std=c++0x -pthread

#O_PARSER = ObjParser.o MatParser.o Loader.o
#OBJECTS = $(O_PARSER) Squelette.o Element3D.o Objet3D.o Face.o Vertex.o VertexNormal.o Material.o Moteur.o
OBJECTS = Clock.o ClockSubscriber.o World.o Wildlife.o Explorer.o Fish.o ScreenViewModel.o ScreenPresenter.o ScreenView.o NCursesView.o
THREAD_OBJECTS = Clock.o
VIEWS_OBJECTS = NCursesView.o

#TESTS_U = test_unitaires
#O_TESTS_U = $(OBJECTS) test_unitaires.o TU_Loader.o TU_Moteur.o TU_MatParser.o

#all:$(EXEC) $(TESTS_U)
all:$(EXEC)


$(EXEC): main.o $(OBJECTS)
	g++ $(OPT) $(INCLUDES) $(OPT_THREAD) o/*.o -o $(BIN)$@ $(LIBS)
	rm o/main.o

$(TESTS_U): $(O_TESTS_U)
	g++ $(OPT) $(INCLUDES) -o $(BIN)$@ o/*.o $(LIBS)

#main.o: main.cpp
#	g++ $(OPT) -c $(INCLUDES) $^ $(LIBS) -o o/$@

NCursesView.o: NCursesView.cpp
	g++ $(OPT) -c $(INCLUDES) $^ $(LIBS) -o o/$@

%.o: %.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o o/$@

%.o: $(SRC_TU)%.cpp
	g++ $(OPT) -c $(INCLUDES) $^ -o o/$@

clean:
	rm o/*.o;find . -name "*~" | xargs rm -f

mrproper: clean
	rm -rf $(EXEC)

install:
	cp $(EXEC) $(INSTALL_DIR)

uninstall:
	rm $(INSTALL_DIR)$(EXEC)

install-libs:
	sudo apt-get install libncurses5 libncursesw5-dev libncurses5-dev ncurses-doc

uninstall-libs:
	sudo apt-get remove libncurses-dev


