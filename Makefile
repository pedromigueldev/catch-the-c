CC=gcc
RAYLIB= -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
CFLAGS=-I. -g -Wall

DEPS = timer.h
OBJ = src/main.o src/timer.o

all: main
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(RAYLIB)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(RAYLIB)

clean:
	rm -f src/*.o main
