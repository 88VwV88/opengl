# ANSI color codes
RESET := \033[0m
RED   := \033[31m
GREEN := \033[32m

pcolor = $($(1))$(2)$(RESET)

CC  := gcc
CXX := g++

WARNINGS := -W -Wall -Wextra -Wconversion

CXXFLAGS := -DGLEW_STATIC $(WARNINGS) -std=c++23 -ggdb -O0 -Iinclude/
CCFLAGS  := -DGLEW_STATIC $(WARNINGS) -std=c23   -ggdb -O0 -Iinclude/

TARGET := opengl
LDFLAGS := -lGL -lglfw -lglm -lz -lassimp

SRC := $(wildcard src/*.c*)
OBJ := $(patsubst src/%, out/%.o, $(SRC))

HEADERS := $(wildcard src/*.h) $(wildcard src/*.hpp)
PCH := $(patsubst src/%, src/%.gch, $(HEADERS))

src/%.h.gch: src/%.h
	@printf "$(GREEN)COMPILING$(RESET) $@\n"
	@$(CC) $(CCFLAGS)   -o $@ $<

src/%.hpp.gch: src/%.hpp
	@printf "$(GREEN)COMPILING$(RESET) $@\n"
	@$(CXX) $(CXXFLAGS) -o $@ $<

out/%.cpp.o: src/%.cpp
	@printf "$(GREEN)COMPILING$(RESET) $@\n"
	@$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

out/%.c.o: src/%.c
	@printf "$(GREEN)COMPILING$(RESET) $@\n"
	@$(CC) $(CCFLAGS)   -c -o $@ $< $(LDFLAGS)

all: setup $(PCH) $(OBJ)
	@printf "$(GREEN) BUILDING$(RESET) $(TARGET)\n"
	@$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)
.PHONY: all

test: all
	@printf "$(GREEN)  RUNNING$(RESET) $(TARGET)\n"
	@gdb ./$(TARGET)
.PHONY: test

run: all
	@printf "$(GREEN)  RUNNING$(RESET) $(TARGET)\n"
	@./$(TARGET)
.PHONY: run

clean:
	@printf "$(RED)CLEANING BUILD FILES$(RESET)\n"
	rm -rf out/* $(TARGET) src/*.gch
.PHONY: clean

setup:
	@mkdir -p out
.PHONY: setup
