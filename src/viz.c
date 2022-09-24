#include "u.h"

#define COL(v)	\
	((v) >> (8*3)) & 0xff,	\
	((v) >> (8*2)) & 0xff,	\
	((v) >> (8*1)) & 0xff,	\
	((v) >> (8*0)) & 0xff

#define UNIT(v)	\
	(int)((v)/LG*W)

#define AGENT_PAD	0.2
#define FOOD_PAD	0.4
#define COL_W		0xffffffff
#define COL_R		0xff0000ff
#define COL_G		0x00ff00ff
#define COL_B		0x00007fff

double dtab[NDIR][6] = {
	[DIR_LEFT]	= {1.0, 0.0, 1.0, 1.0, 0.0, 0.5},
	[DIR_UP]	= {0.0, 0.0, 1.0, 0.0, 0.5, 1.0},
	[DIR_RIGHT]	= {0.0, 0.0, 0.0, 1.0, 1.0, 0.5},
	[DIR_DOWN]	= {0.0, 0.0, 1.0, 0.0, 0.5, 1.0},
};

void drawcell(double i, double j)
{
	int k, p[6];
	Agent *a;
	SDL_Rect wall = {
		UNIT(i)+1,
		UNIT(j)+1,
		UNIT(1.0)-1,
		UNIT(1.0)-1,
	};

	switch(grid[(int)i][(int)j].type) {
	case CELL_AGENT:
		a = grid[(int)i][(int)j].agent;
		for(k = 0; k < 6; k++)
			p[k] = UNIT((k % 2 ? i : j)+dtab[a->dir][k]);
		filledTrigonRGBA(rnd, p[0], p[1], p[2], p[3], p[4], p[5], COL(COL_R));
		aatrigonRGBA(rnd, p[0], p[1], p[2], p[3], p[4], p[5], COL(COL_R));
		break;
	case CELL_FOOD:
		filledCircleRGBA(rnd, UNIT(i+0.5), UNIT(j+0.5), UNIT(0.5-FOOD_PAD/2), COL(COL_G));
		aacircleRGBA(rnd, UNIT(i+0.5), UNIT(j+0.5), UNIT(0.5-FOOD_PAD/2), COL(COL_G));
		aacircleRGBA(rnd, UNIT(i+0.5), UNIT(j+0.5)-1, UNIT(0.5-FOOD_PAD/2), COL(COL_G));
		break;
	case CELL_WALL:
		SDL_SetRenderDrawColor(rnd, COL(COL_B));
		SDL_RenderFillRect(rnd, &wall);
		break;
	}
}

void drawgrid(void)
{
	double i, j;

	SDL_SetRenderDrawColor(rnd, COL(COL_W));
	for(i = 1; i < LG; i++) {
		SDL_RenderDrawLine(rnd, UNIT(i), 0, UNIT(i), H);
		SDL_RenderDrawLine(rnd, 0, UNIT(i), W, UNIT(i));
	}
	for(i = 0; i < LG; i++)
		for(j = 0; j < LG; j++)
			if(grid[(int)i][(int)j].type != CELL_EMPTY)
				drawcell(i, j);
}

int step(void)
{
	input();
	SDL_SetRenderDrawColor(rnd, COL(0));
	SDL_RenderClear(rnd);
	drawgrid();
	SDL_RenderPresent(rnd);
	agentrun();
	return quit;
}

void sim(void)
{
	int i, j;
	Agent a;

	for(i = 0; i < NDIR; i++)
		for(j = 0; j < 6; j++)
			if(dtab[i][j] != 0.5)
				dtab[i][j] = fabs(dtab[i][j]-AGENT_PAD);
	agentinit();
	loop(1000, &step);
}
