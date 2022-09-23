PKG = sdl2 SDL2_gfx
CF = -Werror -ggdb `pkg-config --cflags --libs $(PKG)`
CC = gcc
HF = src/u.h
OBJ = src/lib.o \
      src/cmd.o \
      src/sim.o

%.o:	%.c $(HF)
	$(CC) -o $@ -c $< $(CF)

egen:	$(OBJ) $(HF)
	$(CC) $(OBJ) -o $@ $(CF)

.PHONY:	clean

clean:
	rm -f egen $(OBJ)
