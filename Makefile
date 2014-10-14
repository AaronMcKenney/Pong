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

all: mk_obj_dir $(EXE)

#Make OBJ_DIR if it doesn't already exist
mk_obj_dir:
	mkdir -p $(OBJ_DIR)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

#Every source file has a corresponding object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

#Rebuild objects if any include files change	
$(OBJ): $(INC)

clean:
	rm -rf $(OBJ_DIR) $(EXE)
