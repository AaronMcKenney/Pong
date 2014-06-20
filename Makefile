CC=g++
CFLAGS=-Wall
LDFLAGS=-lSDL -lSDL_ttf

CPP=board.cpp timer.cpp paddle.cpp ball.cpp
INCFLAG=-Iinclude
OBJ=$(patsubst %.cpp, objects/%.o, $(CPP))
SRC=$(patsubst %.cpp, src/%.cpp, $(CPP))

default: pong

pong: $(OBJ) $(SRC) src/pong.cpp
	$(CC) $(CFLAGS) $(INCFLAG) $(OBJ) src/pong.cpp -o $@ $(LDFLAGS)

objects/%.o: src/%.cpp include/%.h
	$(CC) $(CFLAGS) $(INCFLAG) -c $< -o $@ $(LDFLAGS)

clean:
	rm objects/*.o
	rm pong
