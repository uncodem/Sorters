
CPPSOURCE = $(wildcard src/*.cpp)
CPPOBJS = $(patsubst src/%.cpp,build/%.o,$(CPPSOURCE))

EXE := build/sorting
CFLAGS := -Wextra -Wall -Werror -pedantic -MMD -MP -O3
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

CC := clang++

.PHONY: all clean run

all: $(EXE)

run: $(EXE) $(CPPSOURCE)
	@echo -e "\nRunning ... " $< "\n"
	@$<

$(EXE): $(CPPOBJS) | build
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.cpp | build
	$(CC) $(CFLAGS) -c -o $@ $<

build:
	mkdir -p build
	echo "*" > build/.gitignore

clean:
	rm -rf build

