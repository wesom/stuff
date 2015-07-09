#-----------------------------------

CC = gcc
CFLAGS = -g -O2 -Wall
SHARED = -fPIC --shared
INC = -I.
CFLAGS += $(INC)

twosum.so : ltwosum.c
	$(CC) $(CFLAGS) $(SHARED) $^ -o $@

clean:
	rm -fr *.so
