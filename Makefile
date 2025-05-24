CC = gcc
CFLAGS = -Wall

OUTPUT = setter
SOURCES = main.c src/graphics/* src/kernel/*

all: force_build $(OUTPUT)

force_build:
	@if [ -e $(OUTPUT) ]; then rm -f $(OUTPUT); fi

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCES) -g

clean:
	rm -f $(OUTPUT)

.PHONY: all clean force_build