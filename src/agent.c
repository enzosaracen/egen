#include "u.h"

#define NAGENT 5

Agent alist[NAGENT];

int adtab[NDIR][2] = {
	[DIR_LEFT] = 	{-1, 0},
	[DIR_UP] = 	{0, 1},
	[DIR_RIGHT] = 	{1, 0},
	[DIR_DOWN] = 	{0, -1},
};

void agentinit(void)
{
	int i, j, k;
	Agent *a;

	for(i = 0; i < NAGENT; i++) {
		a = &alist[i];
		for(j = 0; j < NTYPE; j++)
			for(k = 0; k < NDIR; k++) {
				do {
					a->act[j][k] = rand() % NACT;
				} while(j == CELL_WALL && alist[i].act[j][k] == ACT_MOVE);
			}
		do {
			a->x = rand() % LG;
			a->y = rand() % LG;
		} while(grid[a->y][a->x].type != CELL_EMPTY);
		grid[a->y][a->x].type = CELL_AGENT;
		grid[a->y][a->x].agent = a;
	}
}

void agentrun(void)
{
	Agent *a;
	int i, nx, ny;

	for(i = 0; i < NAGENT; i++) {
		a = &alist[i];
		nx = a->x+adtab[a->dir][0];
		ny = a->y+adtab[a->dir][1];
		a->front = grid[ny][nx].type;
		if(nx < 0 || nx >= LG || ny < 0 || ny >= LG)
			a->front = CELL_WALL;
		switch(a->act[a->front][a->dir]) {
		case ACT_MOVE:
			grid[a->y][a->x].type = CELL_EMPTY;
			grid[a->y][a->x].agent = NULL;
			a->y += adtab[a->dir][1];
			a->x += adtab[a->dir][0];
			if(a->front == CELL_FOOD)
				a->food++;
			grid[a->y][a->x].type = CELL_AGENT;
			grid[a->y][a->x].agent = a;
			break;
		case ACT_TLEFT:
			a->dir = DIR_LEFT;
			break;
		case ACT_TUP:
			a->dir = DIR_UP;
			break;
		case ACT_TRIGHT:
			a->dir = DIR_RIGHT;
			break;
		case ACT_TDOWN:
			a->dir = DIR_DOWN;
			break;

		}
	}
}
