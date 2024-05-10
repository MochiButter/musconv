SHELL = /bin/sh
SRCDIR = src
TMPDIR = build
BINDIR = bin

EXECNAME = musconv 
MAIN = $(BINDIR)/$(EXECNAME)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(TMPDIR)/%.o)

CC = clang++ 
CFLAGS = -std=c++17 -Wall -Werror -Wextra -pedantic -g -I /usr/include/opus
LIBS = $(shell pkg-config --libs libopenmpt libopusenc)

.PHONY: all clean 

all: $(MAIN)

$(MAIN): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(LIBS) $^ -o $@
	
$(TMPDIR)/%.o: $(SRCDIR)/%.cpp 
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf ./$(BINDIR) ./$(TMPDIR) 
