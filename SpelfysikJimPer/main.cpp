#include "Includes.h"
#include "PhysEntity.h"
#include "CannonBall.h"

int commandHandler(void* data);
SDL_mutex* gBallLock = NULL;
bool exitCond = false; //Only ever modified in commandHandler

int main(int argc, char** argv)
{
	srand(time(NULL));
	SDL_Window* pWnd = nullptr;
	SDL_Surface* pSurf = nullptr;
	SDL_Renderer* pRend = nullptr;
	SDL_Event inEvent;
	gBallLock = SDL_CreateMutex();

	SDL_Init(SDL_INIT_VIDEO);
	pWnd = SDL_CreateWindow("PhysicsFun", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_W, SCR_H, SDL_WINDOW_SHOWN);
	pSurf = SDL_GetWindowSurface(pWnd);
	pRend = SDL_CreateRenderer(pWnd, -1, SDL_RENDERER_ACCELERATED);
	SimpleTimerClass timer;

	SDL_Surface* temp = NULL;
	temp = SDL_LoadBMP("xzbackground.bmp");
	SDL_Rect rdst;
	rdst.x = 0;
	rdst.y = 0;
	rdst.h = SCR_H / 2;
	rdst.w = SCR_W;
	SDL_BlitScaled(temp, NULL, pSurf, &rdst);
	SDL_FreeSurface(temp);
	temp = SDL_LoadBMP("xybackground.bmp");
	rdst.x = 0;
	rdst.y = SCR_H / 2;
	rdst.h = SCR_H / 2;
	rdst.w = SCR_W;
	SDL_BlitScaled(temp, NULL, pSurf, &rdst);
	SDL_FreeSurface(temp);

	CannonBall ball = CannonBall(8.0f, 0.06f, vec3f(10, 0, 0), vec3f(140.0f, 10.0f, 40.0f), vec3f(0.0f, 0.0f, 68.0f));
	SDL_Thread* commandThreadID = SDL_CreateThread(commandHandler, "commandThread", (void*)&ball);
	
	vec3f wind = vec3f(0.0f, -89.0f, 0.0f);
	while (!exitCond)
	{
		timer.update();
		SDL_LockMutex(gBallLock);
		if (ball.launch == true)
		{
			ball.update(timer.deltaTime, wind);
			ball.render(pSurf, pRend);
		}
		SDL_UnlockMutex(gBallLock);
		SDL_UpdateWindowSurface(pWnd);
	}
	SDL_WaitThread(commandThreadID, NULL);
	SDL_DestroyMutex(gBallLock);
	SDL_DestroyWindow(pWnd);
	SDL_DestroyRenderer(pRend);
	SDL_Quit();
	return 0;
}

int commandHandler(void* data)
{
	CannonBall* ball = (CannonBall*)data;
	std::string command;
	std::string args[10];

	std::cout << "Welcome to Cannonball Marksman. For a list of commands, type help" << std::endl;
	while (args[0].compare("exit") != 0)
	{
		std::getline(std::cin, command);
		int index = 0;
		for (int i = 0; i < command.size() && index < 10; i++)
		{
			args[index].clear();
			while (command[i] != ' ' && command[i] != '\0')
			{
				args[index].push_back(command[i++]);
			}
			++index;
		}
		SDL_LockMutex(gBallLock);
		if (args[0].compare("printinfo") == 0)
		{
			ball->printInfo();
		}
		else if (args[0].compare("launch") == 0)
		{
			ball->launch = true;
		}
		else if (args[0].compare("setpos") == 0)
		{
			float x, y, z;
			x = y = z = 0;
			try
			{
				x = std::stof(args[1]);
				y = std::stof(args[2]);
				z = std::stof(args[3]);
			}
			catch(const std::invalid_argument& ia)
			{
				std::cout << "Unable to set position to specified values\n";
				std::cout << args[1] << args[2] << args[3] << std::endl;
			}
			ball->pos.x = x;
			ball->pos.y = y;
			ball->pos.z = z;
		}
		else if (args[0].compare("reset") == 0)
		{
			ball->pos = vec3f(0, 0, 0);
		}
		SDL_UnlockMutex(gBallLock);
	}
	exitCond = true;
	return 0;
}