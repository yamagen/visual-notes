# Makefile for building the generate_index_md C program
# ./index_maker will generate index.md
# ./index_maker ../json/index.json ../index.md
# That's it!
# it requires jansson library for JSON parsing
# Install jansson on Ubuntu: sudo apt-get install libjansson-dev
# on Arch Linux: sudo pacman -S jansson
#
CC = gcc
CFLAGS = -Wall `pkg-config --cflags jansson`
LDFLAGS = `pkg-config --libs jansson`
TARGET = index_maker
#TARGET = ../index.md
SRC = generate_index_md.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
