#include "Includes.h"
#include "PhysEntity.h"
#include "CannonBall.h"

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
	
	CannonBall ball = CannonBall(6.0f, 0.06f, vec3f(0, SCR_W / 4, 0), vec3f(70.0f, 0.0f, 40.0f), vec3f(1, 0.0f, 0));
	vec3f wind = vec3f(0.0f, 14.0f, 0.0f);
	while (1)
	{
		timer.update();
		ball.update(timer.deltaTime, wind);
		ball.render(pSurf, pRend);
		SDL_UpdateWindowSurface(pWnd);
		
	}
	SDL_DestroyWindow(pWnd);
	SDL_DestroyRenderer(pRend);
	SDL_Quit();
	return 0;
}