CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lSDL -lSDL_ttf

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./objects

INC = $(wildcard $(INC_DIR)/*.h)
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

EXE = pong

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

#Every source file has a corresponding object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

#Rebuild objects if any include files change	
$(OBJ): $(INC)

clean:
	rm -f $(OBJ) $(EXE)
