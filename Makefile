CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LFLAGS = -lm

all: tsp

tsp: tsp.o graph.o stack.o path.o
	$(CC) -o tsp tsp.o graph.o path.o stack.o $(LFLAGS)

tsp.o:  tsp.c graph.c graph.h stack.c stack.h path.c path.h vertices.h
	$(CC) $(CFLAGS) -c tsp.c graph.c path.c stack.c

clean:
	rm -f tsp tsp.o graph.o stack.o path.o

format:
	clang-format -i -style=file *.[ch]
