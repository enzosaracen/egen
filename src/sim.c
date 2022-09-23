#include "u.h"

#define COL(v)	\
	((v) >> (8*3)) & 0xff,	\
	((v) >> (8*2)) & 0xff,	\
	((v) >> (8*1)) & 0xff,	\
	((v) >> (8*0)) & 0xff

#define CW	0xffffffff
#define CR	0xff0000ff

#define NG 10

int grid[NG][NG];

void draw(void)
{
	double i, j;

	SDL_SetRenderDrawColor(rnd, COL(CW));
	for(i = 1; i < NG; i++) {
		SDL_RenderDrawLine(rnd, (i/NG)*W, 0, (i/NG)*W, H);
		SDL_RenderDrawLine(rnd, 0, (i/NG)*H, W, (i/NG)*H);
	}

	for(i = 0; i < NG; i++)
		for(j = 0; j < NG; j++)
			switch(grid[(int)i][(int)j]) {
			case 1:
				filledTrigonRGBA(rnd, (j/NG)*W, (i/NG)*H, ((j+1)/NG)*W, (i/NG)*H, (j/NG)*W, ((i+1)/NG)*H, COL(CR));
				break;		
			}
}

int step(void)
{
	input();
	SDL_SetRenderDrawColor(rnd, COL(0));
	SDL_RenderClear(rnd);
	draw();
	SDL_RenderPresent(rnd);
	return quit;
}

void sim(void)
{
	grid[4][4] = 1;
	loop(1000, &step);
}
