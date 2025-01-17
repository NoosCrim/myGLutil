SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

myGLutil.o: ${OBJ_FILES}
	ld -r -o myGLutil.o ${OBJ_FILES}

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp obj
	g++ -c $< -o $@ -I include -O3 -std=c++2b
obj:
	mkdir -p $(OBJ_DIR)


test: myGLutil.o
	g++ test.cpp myGLutil.o -o test -lGL -lglfw -lGLEW -std=c++2b --sanitize=address