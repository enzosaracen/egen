#define EXTERN
#include "u.h"

int alt, inc = 1;

int run(void)
{
	int r, g, b;

	input();
	SDL_SetRenderDrawColor(rnd, alt, alt, alt, 255);
	SDL_RenderClear(rnd);
	SDL_RenderPresent(rnd);
	if(alt+inc > 255 || alt+inc < 0)
		inc = -inc;
	alt += inc;

	return quit;
}

int main(void)
{
	init();
	loop(10, &run);
	SDL_Quit();
}
