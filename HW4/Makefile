CFLAGS = -Wall -O2
CC = gcc

source=$(wildcard *.c)

obj = $(source:.c=)

all: $(obj)

$(obj): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(obj)
