#define EXTERN
#include "u.h"

int main(void)
{
	srand(time(NULL));
	init();
	start();
	SDL_Quit();
}
