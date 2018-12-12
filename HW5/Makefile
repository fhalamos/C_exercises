CFLAGS = -Wall -O2 -fopenmp -lm -std=gnu99
CC = gcc

source=$(wildcard *.c)

obj = $(source:.c=)

all: $(obj)

$(obj): %: %.c
	$(CC) $(CFLAGS) -o $@ $< -lm

clean:
	rm -f $(obj)