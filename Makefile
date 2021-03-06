CC=g++
CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -I $(IDIR) -g -Wall -std=c++11

IDIR = include
ODIR = obj
SDIR = src

_DEPS = map.h pathfinder.h node.h util.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = map.o pathfinder.o node.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o bsearch $(SDIR)/main.cpp $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
