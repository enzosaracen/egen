#include "u.h"

void errorf(char *fmt, ...)
{
	va_list arg;

	fprintf(stderr, "error: ");
	va_start(arg, fmt);
	vfprintf(stderr, fmt, arg);
	va_end(arg);
	fprintf(stderr, "\n");
	SDL_Quit();
	exit(1);
}

void input(void)
{
	SDL_Event e;

	while(SDL_PollEvent(&e))
		switch(e.type) {
		case SDL_QUIT:
			quit = 1;
			break;
		}
}

void loop(uint32 ms, int (*run)(void))
{
	int t, dt, err;

	err = 0;
	for(;;) {
		t = SDL_GetTicks();
		if((*run)())
			break;

		dt = SDL_GetTicks()-t;
		if(dt < ms) {
			err -= ms-dt;
			if(err < 0) {
				SDL_Delay(-err);
				err = 0;
			}
		} else if(dt > ms)
			err += dt-ms;
	}
}

void init(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	scr = SDL_CreateWindow("gpx", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_OPENGL);
	rnd = SDL_CreateRenderer(scr, -1, 0);
}
