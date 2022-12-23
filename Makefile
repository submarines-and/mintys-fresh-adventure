UNAME_S = $(shell uname -s)

CXX = clang++
CXXFLAGS = -Wall -Wextra --std=c++17 -g
CXXFLAGS += -Ilib/glad/include -Ilib/glfw/include -Ilib/cglm/include

LDFLAGS = lib/glad/src/glad.o lib/glfw/src/libglfw3.a lib/cglm/libcglm.a

# GLFW required frameworks on OSX
ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
endif

ifeq ($(UNAME_S), Linux)
	LDFLAGS += -ldl -lpthread
endif

SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) 
OBJ = $(SRC:.cpp=.o)
BIN = bin

.PHONY: start build clean install

start: build
	$(BIN)/game

build: $(OBJ)
	mkdir -p ./$(BIN)
	$(CXX) -o $(BIN)/game $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)

install:
	git submodule deinit -f . && git submodule update --init

	git update-index --assume-unchanged lib/cglm
	git update-index --assume-unchanged lib/glfw

	cd lib/cglm && cmake . -DCGLM_STATIC=ON && make
	cd lib/glad && $(CXX) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/glfw && cmake . && make