UNAME_S = $(shell uname -s)

CXX = clang++

CXXFLAGS = -Wall -Wextra --std=c++17 -g -MMD -O1
CXXFLAGS += -I./src -Ilib/glad/include -Ilib/glfw/include -Ilib/glm -Ilib/stb -Ilib/tinyobjloader -Ilib/FastNoiseLite/Cpp

LDFLAGS = lib/glad/src/glad.o lib/glfw/src/libglfw3.a


# GLFW required frameworks on OSX
ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
endif

ifeq ($(UNAME_S), Linux)
	LDFLAGS += -ldl -lpthread
endif

SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp) 
OBJ = $(SRC:.cpp=.o)
DEPS = $(OBJ:.o=.d)
BIN = bin

.PHONY: start build clean libs

start: build
	$(BIN)/MintyFresh

build: $(OBJ)
	mkdir -p ./$(BIN)
	$(CXX) -o $(BIN)/MintyFresh $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(BIN) $(OBJ) $(DEPS)

libs:
# Reload and ignore output changes
	git submodule deinit -f . && git submodule update --init
	git update-index --assume-unchanged lib/glfw

# Glad and glfw 
	cd lib/glad && $(CXX) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/glfw && cmake . && make

-include $(DEPS)


# parallell builds
NPROCS = $(shell sysctl hw.ncpu  | grep -o '[0-9]\+')
MAKEFLAGS += -j$(NPROCS)