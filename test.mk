CC = gcc
FLAGS = -g -c

SOURCEDIR = /src/code
BUILDDIR = build

EXECUTABLE = xunmadb
SOURCES = $(wildcard src/unit/*_.c)
OBJECTS = $(patsubst src/unit/*_.c,build/%.o,$(SOURCES))

all: dir $(BUILDDIR)/$(EXECUTABLE)

dir:
    mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(EXECUTABLE): $(OBJECTS)
    $(CC) $^ -o $@

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
    $(CC) $(FLAGS) $< -o $@

.PHONY : clean
clean:
    rm -f $(BUILDDIR)/*o $(BUILDDIR)/$(EXECUTABLE)
