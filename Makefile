CC=g++ -std=c++0x
INCS=/home/kyle/SFML/include
PROJ_INC=Include/
LIBS=/home/kyle/SFML/lib
SFML_LINKS=-lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRCS=$(wildcard Source/*.cpp)
OBJS=$(SRCS:Source/*.cpp=*.o)
OBJS_SRC=$(patsubst %.cpp,%.o, $(SRCS))
OBJS_DIR=Objs
EXNM=tranks

all: compile link
debug: db dblink

compile:
	$(CC) $(SRCS) -I$(INCS) -I$(PROJ_INC) -c #$< -o $(OBJS_DIR)

link:
	$(CC) $(SRCS) -I$(INCS) -I$(PROJ_INC) -o $(EXNM) -L$(LIBS) $(SFML_LINKS)

db:
	$(CC) -g $(SRCS) -I$(INCS) -I$(PROJ_INC) -c

dblink:
	$(CC) -g $(SRCS) -I$(INCS) -I$(PROJ_INC) -o $(EXNM) -L$(LIBS) $(SFML_LINKS)

butt:
	$(CC) $(SRCS) -I$(INCS) -I$(PROJ_INC) -o $(EXNM) -L$(LIBS) $(SFML_LINKS)

cleano:
	rm *.o

clean:
	rm *.o $(EXNM)
