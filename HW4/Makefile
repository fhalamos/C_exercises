#CFLAGS = -g
CFLAGS = -Wall -O2
CC = gcc

source=$(wildcard *.c)

obj = $(source:.c=)

all: $(obj)

$(obj): %: %.c
	$(CC) $(CFLAGS) -o $@ $< -lm

clean:
	rm -f $(obj)