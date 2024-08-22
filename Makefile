CC := gcc
CFLAGS := -Wall -Wextra

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)

all: PrintMCUCan

PrintMCUCan: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f PrintMCUCan $(OBJECTS)