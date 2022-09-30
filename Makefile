CC=gcc
CFLAGS= -Wall -Wextra -I.
DEPS = base.h
OBJ = main.c my_tar.c
TARGET = my_tar
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

fclean:
	rm -f ${TARGET} *.o
