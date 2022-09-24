#define EXTERN
#include "u.h"

int main(void)
{
	srand(time(NULL));
	init();
	sim();
	SDL_Quit();
}
