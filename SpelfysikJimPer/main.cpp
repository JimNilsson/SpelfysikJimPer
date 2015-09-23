#include "Includes.h"
#include "PhysEntity.h"
#include "CannonBall.h"
#include "RopeBall.h"
#include <fstream>

int commandHandler(void* data);
void set3f(float& x, float& y, float& z, std::string* args);//Helper for commandHandler
void clearBackground();
void cmdHelp(std::string* args, CannonBall* ball);

SDL_mutex* gBallLock = NULL;
SDL_mutex* gRenderLock = NULL;

SDL_Surface* gSurf = nullptr;

bool exitCond = false; //Only ever modified in commandHandler
vec3f wind = vec3f(0.0f, 0.0f, 0.0f); // same as above

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	SDL_Window* pWnd = nullptr;
	SDL_Renderer* pRend = nullptr;

	gBallLock = SDL_CreateMutex();
	gRenderLock = SDL_CreateMutex();

	SDL_Init(SDL_INIT_VIDEO);
	pWnd = SDL_CreateWindow("Cannon Ball Marksman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_W, SCR_H, SDL_WINDOW_SHOWN);
	gSurf = SDL_GetWindowSurface(pWnd);
	pRend = SDL_CreateRenderer(pWnd, -1, SDL_RENDERER_ACCELERATED);
	SimpleTimerClass timer;

	clearBackground();

	CannonBall ball = CannonBall(8.0f, 0.06f, vec3f(10, 0, 0), vec3f(140.0f, 10.0f, 40.0f), vec3f(0.0f, 0.0f, 68.0f));
	SDL_Thread* commandThreadID = SDL_CreateThread(commandHandler, "commandThread", (void*)&ball);
	RopeBall rball = RopeBall(800.0f, 1.0f, 80.0f, vec3f(600.0f, 0.0f, 300.0f));
	
	while (!exitCond)
	{
		timer.update();
		rball.update((float)timer.deltaTime, wind);
		
		SDL_LockMutex(gBallLock);
		if (ball.launch == true)
		{
			ball.update((float)timer.deltaTime, wind);
		}
		SDL_UnlockMutex(gBallLock);

		SDL_LockMutex(gRenderLock);
		
		clearBackground();
		ball.render(gSurf, pRend);
		rball.render(gSurf, pRend);
		SDL_UpdateWindowSurface(pWnd);

		SDL_UnlockMutex(gRenderLock);
		
	}
	SDL_WaitThread(commandThreadID, NULL);
	SDL_DestroyMutex(gBallLock);
	SDL_DestroyMutex(gRenderLock);
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
		for (unsigned int i = 0; i < command.size() && index < 10; i++)
		{
			args[index].clear();
			while (command[i] != ' ' && command[i] != '\0')
			{
				args[index].push_back(command[i++]);
			}
			++index;
		}
		SDL_LockMutex(gBallLock);
		cmdHelp(args, ball);
		SDL_UnlockMutex(gBallLock);
	}
	exitCond = true;
	return 0;
}

void cmdHelp(std::string* args, CannonBall* ball)
{
	if (args[0].compare("help") == 0)
	{
		printf("exit                Exits the program\n");
		printf("help                Lists available commands.\n");
		printf("printinfo           Prints velocity and force acting on the cannonball\n");
		printf("setlinvel x y z     Sets initial velocity of cannonball\n");
		printf("setangvel x y z     Sets angular velocity of ball\n");
		printf("setwind x y z       Sets wind velocity\n");
		printf("setmass m           Sets mass of cannonball\n");
		printf("reset               Sets all values to zero.\n");
		printf("launch              Launches the ball\n");
		printf("clear               Clears the trajectory from screen\n");
		
	}
	else if (args[0].compare("printinfo") == 0)
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
		set3f(x, y, z, args);
		ball->pos.x = x;
		ball->pos.y = y;
		ball->pos.z = z;
	}
	else if (args[0].compare("reset") == 0)
	{
		ball->pos = vec3f(0, 0, 0);
		ball->linVel = vec3f(0, 0, 0);
		ball->launch = false;
		wind = vec3f(0, 0, 0);
		ball->angVel = vec3f(0, 0, 0);
	}
	else if (args[0].compare("setlinvel") == 0)
	{
		float x, y, z;
		set3f(x, y, z, args);
		ball->linVel.x = x;
		ball->linVel.y = y;
		ball->linVel.z = z;
	}
	else if (args[0].compare("setangvel") == 0)
	{
		float x, y, z;
		set3f(x, y, z, args);
		ball->angVel.x = x;
		ball->angVel.y = y;
		ball->angVel.z = z;
	}
	else if (args[0].compare("setwind") == 0)
	{
		float x, y, z;
		set3f(x, y, z, args);
		wind.x = x;
		wind.y = y;
		wind.z = z;
	}
	else if (args[0].compare("setmass") == 0)
	{
		float x = 0;
		x = stof(args[1]);
		if (x > EPSILON)
			ball->mass = x;
		else
			printf("Mass too low\n");
	}
	else if (args[0].compare("clear") == 0)
	{
		SDL_LockMutex(gRenderLock);
		clearBackground();
		SDL_UnlockMutex(gRenderLock);
	}
	else
	{
		printf("Unrecognized command\n");
	}
}

void set3f(float& x, float& y, float& z, std::string* args)
{
	x = y = z = 0;
	try
	{
		x = std::stof(args[1]);
		y = std::stof(args[2]);
		z = std::stof(args[3]);
	}
	catch (const std::invalid_argument& ia)
	{
		std::cout << "Unable to set position to specified values\n";
		std::cout << ia.what() << std::endl;
	}
}

void clearBackground()
{
	SDL_Surface* temp = NULL;
	temp = SDL_LoadBMP("xzbackground.bmp");
	SDL_Rect rdst;
	rdst.x = 0;
	rdst.y = 0;
	rdst.h = SCR_H / 2;
	rdst.w = SCR_W;
	SDL_BlitScaled(temp, NULL, gSurf, &rdst);
	SDL_FreeSurface(temp);
	temp = SDL_LoadBMP("xybackground.bmp");
	rdst.x = 0;
	rdst.y = SCR_H / 2;
	rdst.h = SCR_H / 2;
	rdst.w = SCR_W;
	SDL_BlitScaled(temp, NULL, gSurf, &rdst);
	SDL_FreeSurface(temp);
}
