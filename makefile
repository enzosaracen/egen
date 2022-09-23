CF = -Werror -ggdb $(shell sdl2-config --libs --cflags)
CC = gcc
HF = src/u.h
OBJ = src/lib.o \
      src/cmd.o

%.o:	%.c $(HF)
	$(CC) -o $@ -c $< $(CF)

egen:	$(OBJ) $(HF)
	$(CC) $(OBJ) -o $@ $(CF)

.PHONY:	clean

clean:
	rm -f egen $(OBJ)
