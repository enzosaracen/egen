#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>

#define H	500
#define W	500
#define LG	50
#define ITER	100
#define MUTATE	50	

#define NAGENT	LG
#define NFOOD	((LG*LG)*0.4)

#define NWALL	0

#ifndef EXTERN
#define EXTERN extern
#endif

typedef uint32_t uint32;
typedef struct Agent Agent;
typedef struct Cell Cell;

enum {
	CELL_EMPTY = 0,
	CELL_AGENT,
	CELL_FOOD,
	CELL_WALL,
	NTYPE,
};

enum {
	DIR_LEFT = 0,
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	NDIR,
};

enum {
	ACT_TLEFT,
	ACT_TUP,
	ACT_TRIGHT,
	ACT_TDOWN,
	ACT_MOVE,
	NACT,
};

struct Agent {
	int	food;
	int	dir;
	int	front;
	int	act[NTYPE][NDIR];
	int	x, y;
};

struct Cell {
	int	type;
	Agent	*agent;
};

/*
 *	lib.c
 */
void	errorf(char *, ...);
void	input(void);
void	loop(uint32, int (*)(void));
void	init(void);

/*
 *	viz.c
 */
void	start(void);

/*
 *	agent.c
 */
void	agentinit(void);
void	agentbreed(void);
void	gridclear(void);
void	gridinit(void);
void	agentrun(void);

EXTERN	SDL_Window	*scr;
EXTERN	SDL_Renderer	*rnd;
EXTERN	int		quit;
EXTERN	Cell		grid[LG][LG];
EXTERN	int		foodtotal;
