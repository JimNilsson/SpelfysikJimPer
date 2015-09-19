#include "Includes.h"

int main(int argc, char** argv)
{
	srand(time(NULL));
	SDL_Window* pWnd = nullptr;
	SDL_Surface* pSurf = nullptr;
	SDL_Renderer* pRend = nullptr;
	SDL_Event inEvent;


	SDL_Init(SDL_INIT_VIDEO);
	pWnd = SDL_CreateWindow("PhysicsFun", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_W, SCR_H, SDL_WINDOW_SHOWN);
	pSurf = SDL_GetWindowSurface(pWnd);
	pRend = SDL_CreateRenderer(pWnd, -1, SDL_RENDERER_ACCELERATED);
	SimpleTimerClass timer;

	while (1);
	
	return 0;
}