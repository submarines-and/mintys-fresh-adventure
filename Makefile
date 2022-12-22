UNAME_S = $(shell uname -s)

CPPFLAGS = -Wall -Wextra -I./src -Ilib/glad/include -Ilib/glfw/include --std=c++17
LDFLAGS = lib/glad/src/glad.o lib/glfw/src/libglfw3.a

# GLFW required frameworks on OSX
ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
endif

ifeq ($(UNAME_S), Linux)
	LDFLAGS += -ldl -lpthread
endif

SRC  = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)  $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp) 
OBJ  = $(SRC:.cpp=.o)
BIN = bin

clean:
	rm -rf $(BIN) $(OBJ)
	mkdir -p ./$(BIN)

install:
	git submodule deinit -f . && git submodule update --init
	git update-index --assume-unchanged lib/glfw

	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/glfw && cmake . && make

build: $(OBJ)
	mkdir -p ./$(BIN)
	clang++ -o $(BIN)/game $^ $(LDFLAGS) -v

start: clean build
	$(BIN)/game


