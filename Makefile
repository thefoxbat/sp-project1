CC = gcc

CFLAGS = -Wall -Wextra -g
AR = ar
ARFLAGS = rcs

OBJS = memory_manager.o bitmap.o

LIB = libmemory_manager.a

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

memory_manager.o: memory_manager.c memory_manager.h bitmap.h
	$(CC) $(CFLAGS) -c memory_manager.c

bitmap.o: bitmap.c bitmap.h
	$(CC) $(CFLAGS) -c bitmap.c	

test: test_main.c $(LIB)
	$(CC) $(CFLAGS) -o test_main test_main.c -L. -lmemory_manager

clean:
	del /Q $(OBJS) $(LIB) test_main.exe 2>nul || exit 0

.PHONY: all clean test

