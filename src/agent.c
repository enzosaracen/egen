#include "u.h"

Agent alist[NAGENT];

int adtab[NDIR][2] = {
	[DIR_LEFT] = 	{-1, 0},
	[DIR_UP] = 	{0, -1},
	[DIR_RIGHT] = 	{1, 0},
	[DIR_DOWN] = 	{0, 1},
};

int randact(int j, int k)
{
	int a;

	do {
		a = rand() % NACT;
	} while(((j == CELL_WALL || j == CELL_AGENT) && a == ACT_MOVE)
		|| (a == k));
	return a;
}

void agentinit(void)
{
	int i, j, k;
	Agent *a;

	for(i = 0; i < NAGENT; i++) {
		a = &alist[i];
		for(j = 0; j < NTYPE; j++)
			for(k = 0; k < NDIR; k++)
				alist[i].act[j][k] = randact(j, k);
		do {
			a->x = rand() % LG;
			a->y = rand() % LG;
		} while(grid[a->y][a->x].type != CELL_EMPTY);
		grid[a->y][a->x].type = CELL_AGENT;
		grid[a->y][a->x].agent = a;
	}
}

void agentbreed(void)
{
	int i, j, k, sum, dist[(int)NFOOD];
	int abuf[NAGENT][NDIR][NTYPE];

	sum = 0;
	for(i = 0; i < NAGENT; i++) {
		for(j = sum; j < sum+alist[i].food; j++)
			dist[j] = i;
		sum += alist[i].food;
	}
	if(sum == 0) {
		agentinit();
		return;
	}
	for(i = 0; i < NAGENT; i++) {
		for(j = 0; j < NTYPE; j++)
			for(k = 0; k < NDIR; k++) {
				if(rand()%MUTATE == 0)
					abuf[i][j][k] = randact(j, k);
				else
					abuf[i][j][k] = alist[dist[rand()%sum]].act[j][k];
			}
	}
	for(i = 0; i < NAGENT; i++) {
		alist[i].food = 0;
		for(j = 0; j < NTYPE; j++)
			for(k = 0; k < NDIR; k++)
				alist[i].act[j][k] = abuf[i][j][k];
		do {
			alist[i].x = rand() % LG;
			alist[i].y = rand() % LG;
		} while(grid[alist[i].y][alist[i].x].type != CELL_EMPTY);
		grid[alist[i].y][alist[i].x].type = CELL_AGENT;
		grid[alist[i].y][alist[i].x].agent = &alist[i];
	}
}

/*
void agentbreed(void)
{
	int i, j, k, m1, m2, sum;
	int act1[NTYPE][NDIR], act2[NTYPE][NDIR];

	Agent acpy[A]
	
	m1 = m2 = -1;
	for(i = 0; i < NAGENT; i++) {
		if(alist[i].food > m1) {
			if(m1 > 0) {
				m2 = m1;
				a2 = a1;
			}
			m1 = alist[i].food;
			a1 = &alist[i];
		} else if(alist[i].food > m2) {
			m2 = alist[i].food;
			a2 = &alist[i];
		}
	}
	for(i = 0; i < NTYPE; i++)
		for(j = 0; j < NDIR; j++) {
			act1[i][j] = a1->act[i][j];
			act2[i][j] = a2->act[i][j];
		}
	for(i = 0; i < NAGENT; i++) {
		a1 = &alist[i];
		a1->food = a1->dir = 0;
		for(j = 0; j < NTYPE; j++)
			for(k = 0; k < NDIR; k++) {
				if(rand()%MUTATE == 0)
					a1->act[j][k] = randact(j, k);
				else
					a1->act[j][k] = rand() & 1 ? act1[j][k] : act2[j][k];
			}

		do {
			a1->x = rand() % LG;
			a1->y = rand() % LG;
		} while(grid[a1->y][a1->x].type != CELL_EMPTY);
		grid[a1->y][a1->x].type = CELL_AGENT;
		grid[a1->y][a1->x].agent = a1;
	}
}*/

void gridclear(void)
{
	int i, j;

	for(i = 0; i < LG; i++)
		for(j = 0; j < LG; j++) {
			grid[i][j].type = CELL_EMPTY;
			grid[i][j].agent = NULL;
		}
}

void gridinit(void)
{
	int i, j, k;
	Agent *a;

	foodtotal = 0;
	for(i = 0; i < NFOOD; i++) {
		do {
			j = rand() % LG;
			k = rand() % LG;
		} while(grid[j][k].type != CELL_EMPTY);
		grid[j][k].type = CELL_FOOD;
	}
	for(i = 0; i < NWALL; i++) {
		do {
			j = rand() % LG;
			k = rand() % LG;
		} while(grid[j][k].type != CELL_EMPTY);
		grid[j][k].type = CELL_WALL;
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
			if(a->front == CELL_FOOD) {
				a->food++;
				foodtotal++;
			}
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
