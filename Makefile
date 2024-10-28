SHELL = /bin/sh
SRCDIR = src
INCDIR = include
TMPDIR = build

EXECNAME = musconv 
MAIN = $(EXECNAME)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
HEADERS = $(wildcard $(SRCDIR)/*.h)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(TMPDIR)/%.o)

#CC = clang++ 
CC = g++ 
CFLAGS = -std=c++17 -Wall -Werror -Wextra -pedantic -g -I /usr/include/opus -I include
LIBS = $(shell pkg-config --libs libopenmpt libopusenc)

.PHONY: all clean 

all: $(MAIN)

$(MAIN): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $^ $(LIBS) -o $@
	
$(TMPDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf ./$(MAIN) ./$(TMPDIR) 
