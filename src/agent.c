#include "u.h"

Agent alist[NAGENT];

int adtab[NDIR][2] = {
	[DIR_LEFT] = 	{-1, 0},
	[DIR_UP] = 	{0, -1},
	[DIR_RIGHT] = 	{1, 0},
	[DIR_DOWN] = 	{0, 1},
};

int randact(int f)
{
	int a;

	do {
		a = rand() % NACT;
	} while(((f == CELL_WALL || f == CELL_AGENT) && a == ACT_MOVE));
	return a;
}

void agentinit(void)
{
	int i, j, k;
	Agent *a;

	for(i = 0; i < NAGENT; i++) {
		a = &alist[i];
		for(j = 0; j < NTYPE; j++)
			alist[i].act[j] = randact(j);
		do {
			a->x = rand() % LG;
			a->y = rand() % LG;
		} while(grid[a->y][a->x].type != CELL_EMPTY);
		grid[a->y][a->x].type = CELL_AGENT;
		grid[a->y][a->x].agent = a;
	}
}

void agentdump(void)
{
	int i, j, k, max;
	FILE *f;
	Agent *a;

	max = -1;
	for(i = 0; i < NAGENT; i++)
		if(alist[i].food > max) {
			max = alist[i].food;
			a = &alist[i];
		}
	f = fopen("agent.out", "w");
	for(i = 0; i < NTYPE; i++) {
		switch(i) {
		case CELL_EMPTY:
			fprintf(f, "cell_empty: ");
			break;
		case CELL_AGENT:
			fprintf(f, "cell_agent: ");
			break;
		case CELL_FOOD:
			fprintf(f, "cell_food: ");
			break;
		case CELL_WALL:
			fprintf(f, "cell_wall: ");
			break;
		}
		switch(a->act[i]) {
		case ACT_TLEFT:
			fprintf(f, "turn left\n");
			break;
		case ACT_TRIGHT:
			fprintf(f, "turn right\n");
			break;
		case ACT_MOVE:
			fprintf(f, "move\n");
			break;
		}
	}
	fclose(f);
}

void agentbreed(void)
{
	int i, j, k, sum, dist[(int)NFOOD];
	int abuf[NAGENT][NTYPE];

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
	for(i = 0; i < NAGENT; i++)
		for(j = 0; j < NTYPE; j++) {
			if(rand()%MUTATE == 0)
				abuf[i][j] = randact(j);
			else
				abuf[i][j] = alist[dist[rand()%sum]].act[j];
		}
	for(i = 0; i < NAGENT; i++) {
		alist[i].food = 0;
		for(j = 0; j < NTYPE; j++)
			alist[i].act[j] = abuf[i][j];
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
	Agent *a1, *a2;

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
		switch(a->act[a->front]) {
		case ACT_MOVE:
			grid[a->y][a->x].type = CELL_EMPTY;
			grid[a->y][a->x].agent = NULL;
			a->y = ny;
			a->x = nx;
			if(a->front == CELL_FOOD) {
				a->food++;
				foodtotal++;
			}
			grid[a->y][a->x].type = CELL_AGENT;
			grid[a->y][a->x].agent = a;
			break;
		case ACT_TLEFT:
			if(--a->dir < 0)
				a->dir = DIR_DOWN;
			break;
		case ACT_TRIGHT:
			if(++a->dir >= NDIR)
				a->dir = DIR_LEFT;
			break;
		}
	}
}
