CC=g++ -std=c++0x
INCS=/home/kyle/SFML/include
#INCS2=/home/kyle/workspace/SFMLtest/examples/03_World/Include/
INCS2=Include/
LIBS=/home/kyle/SFML/lib
#SRCS=Main.cpp Game.cpp Aircraft.cpp Entity.cpp SceneNode.cpp SpriteNode.cpp World.cpp
SRCS=$(wildcard Source/*.cpp)
#OBJS=$(SRCS:.cpp=.o)
OBJS=$(SRCS:Source/*.cpp=*.o)
OBJS_SRC=$(patsubst %.cpp,%.o, $(SRCS))
OBJS_DIR=Objs
EXNM=tranks

all: compile link
debug: db dblink

compile:
#	mkdir -p $(OBJS_DIR)
	$(CC) $(SRCS) -I$(INCS) -I$(INCS2) -c #$< -o $(OBJS_DIR)
#	$(CC) -c $(SRCS) -I$(INCS) -I$(INCS2) -o $@ $<

link:
	$(CC) $(SRCS) -I$(INCS) -I$(INCS2) -o $(EXNM) -L$(LIBS) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
#	$(CC) $(OBJS) -o $(EXNM) -L$(LIBS) -lsfml-graphics -lsfml-window -lsfml-system

db:
	$(CC) -g $(SRCS) -I$(INCS) -I$(INCS2) -c

dblink:
	$(CC) -g $(SRCS) -I$(INCS) -I$(INCS2) -o $(EXNM) -L$(LIBS) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

butt:
	$(CC) $(SRCS) -I$(INCS) -I$(INCS2) -o $(EXNM) -L$(LIBS) -lsfml-graphics -lsfml-window -lsfml-system

cleano:
	rm *.o

clean:
	rm *.o $(EXNM)
#	rm $(OBJS_DIR)/*.o $(EXNM)
