#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>

#define H 500
#define W 500

#ifndef EXTERN
#define EXTERN extern
#endif

typedef uint32_t uint32;

/*
 *	lib.c
 */
void	errorf(char *, ...);
void	input(void);
void	loop(uint32, int (*)(void));
void	init(void);

/*
 *	sim.c
 */
void	sim(void);

EXTERN	SDL_Window	*scr;
EXTERN	SDL_Renderer	*rnd;
EXTERN	int		quit;
