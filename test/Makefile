CC = gcc
CFLAGS = -Wall -Wpedantic -Wextra \
	 -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function \
	 -march=native -std=c99
TEST = -Og -g
PROD = -O3

DEPS = ../flags.h unittest.c test.c

test: test.out
.PHONY: test

test.out: test.c $(DEPS)
	$(CC) $(CFLAGS) $(TEST) $< -o $@ && ./$@

clean:
	rm *.out
