#-----------------------------------

CC = gcc
CFLAGS = -g -O2 -Wall
SHARED = -fPIC --shared
INC = -I.
CFLAGS += $(INC)

TAR = sametree
$(TAR).so : $(TAR).c
	$(CC) $(CFLAGS) $(SHARED) $^ -o $@

clean:
	rm -fr *.so
