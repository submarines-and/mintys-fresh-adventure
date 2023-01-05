UNAME_S = $(shell uname -s)

CXX = clang++

CXXFLAGS = -Wall -Wextra --std=c++17 -g
CXXFLAGS += -I./src -Ilib/glad/include -Ilib/glfw/include -Ilib/glm -Ilib/stb -Ilib/assimp/include

LDFLAGS = lib/glad/src/glad.o lib/glfw/src/libglfw3.a lib/assimp/lib/libassimp.a -lz

# GLFW required frameworks on OSX
ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
endif

ifeq ($(UNAME_S), Linux)
	LDFLAGS += -ldl -lpthread
endif

SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp) 
OBJ = $(SRC:.cpp=.o)
BIN = bin

.PHONY: start build clean install

start: build
	$(BIN)/MintyFresh

build: $(OBJ)
	mkdir -p ./$(BIN)
	$(CXX) -o $(BIN)/MintyFresh $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)

install:
# Reload and ignore output changes
	git submodule deinit -f . && git submodule update --init
	git update-index --assume-unchanged lib/glfw

# Glad and glfw 
	cd lib/glad && $(CXX) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/glfw && cmake . && make

# Assimp
	cd lib/assimp && cmake CMakeLists.txt -D BUILD_SHARED_LIBS=OFF -D ASSIMP_INSTALL=OFF -D ASSIMP_WARNINGS_AS_ERRORS=OFF -D CMAKE_INSTALL_PREFIX=bin && cmake --build . 
	