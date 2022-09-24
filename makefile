PKG = sdl2 SDL2_gfx
CF = -Werror -ggdb `pkg-config --cflags --libs $(PKG)` -lm
CC = gcc
HF = src/u.h
OBJ = src/lib.o \
      src/cmd.o \
      src/viz.o \
      src/agent.o

%.o:	%.c $(HF)
	$(CC) -o $@ -c $< $(CF)

egen:	$(OBJ) $(HF)
	$(CC) $(OBJ) -o $@ $(CF)

.PHONY:	clean

clean:
	rm -f egen $(OBJ)
