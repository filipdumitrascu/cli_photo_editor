# Copyright 2023 < 323CA Dumitrascu Filip Teodor >

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -std=c99

# define targets
TARGETS=image_editor

build:
	$(CC) $(CFLAGS) *.c -o $(TARGETS) -lm

pack:
	zip -FSr 323CA_DumitrascuFilipTeodor_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean