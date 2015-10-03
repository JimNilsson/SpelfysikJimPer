#include "Includes.h"
#include "PhysEntity.h"
#include "CannonBall.h"
#include "RopeBall.h"
#include <fstream>

int commandHandler(void* data);
void set3f(float& x, float& y, float& z, std::string* args);//Helper for commandHandler
void initBGTex();
void clearBackground();
void cmdHelp(std::string* args, CannonBall* ball);
void Kollision(CannonBall* cBall, RopeBall* rBall);

SDL_mutex* gBallLock = NULL;
SDL_mutex* gRenderLock = NULL;

SDL_Surface* gSurf = nullptr;
SDL_Window* gWnd = nullptr;
SDL_Renderer* gRend = nullptr;
SDL_Texture* gBGTex = NULL;

bool exitCond = false; //Only ever modified in commandHandler
vec3f wind = vec3f(0.0f, 0.0f, 0.0f); // same as above

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));

	gBallLock = SDL_CreateMutex();
	gRenderLock = SDL_CreateMutex();

	SDL_Init(SDL_INIT_VIDEO);
	gWnd = SDL_CreateWindow("Cannonball Marksman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_W, SCR_H, SDL_WINDOW_SHOWN);
	gSurf = SDL_GetWindowSurface(gWnd);
	gRend = SDL_CreateRenderer(gWnd, -1, SDL_RENDERER_ACCELERATED);
	SimpleTimerClass timer;
	initBGTex();

	clearBackground();

	CannonBall ball = CannonBall(263860.0f, 2.0f, vec3f(10, 0, 1.0f), vec3f(140.0f, 10.0f, 40.0f), vec3f(0.0f, 0.0f, 68.0f));
	SDL_Thread* commandThreadID = SDL_CreateThread(commandHandler, "commandThread", (void*)&ball);
	RopeBall rball = RopeBall(160000.0f, 8.0f, 80.0f, vec3f(600.0f, 0.0f, 300.0f));
	
	while (!exitCond)
	{
		timer.update();
		rball.update((float)timer.deltaTime, wind);
		
		SDL_LockMutex(gBallLock);
		if (ball.launch == true)
		{
			Kollision(&ball, &rball);
			ball.update((float)timer.deltaTime, wind);
		}
		SDL_UnlockMutex(gBallLock);

		SDL_LockMutex(gRenderLock);
		
		clearBackground();
		ball.render();
		rball.render();
		SDL_RenderPresent(gRend);

		SDL_UnlockMutex(gRenderLock);
		
	}
	SDL_WaitThread(commandThreadID, NULL);
	SDL_DestroyMutex(gBallLock);
	SDL_DestroyMutex(gRenderLock);
	SDL_DestroyTexture(gBGTex);
	SDL_DestroyWindow(gWnd);
	SDL_DestroyRenderer(gRend);
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
		printf("setradius r         Sets radius of cannonball");
		printf("reset               Sets position, lin. velocity, ang. velocity and wind to 0.\n");
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
		ball->pos = vec3f(0, 0, ball->radius);
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
		{
			ball->mass = x;
			ball->gravForce = vec3f(0.0f, 0.0f, ball->mass * -1.0f * GRAVACC);
		}
		else
			printf("Mass too low\n");
	}
	else if (args[0].compare("setradius") == 0)
	{
		float x = 0;
		x = stof(args[1]);
		if (x > EPSILON)
		{
			ball->radius = x;
			ball->area = ball->radius * ball->radius * PI;
		}
		else
			printf("Radius too low\n");
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


	SDL_RenderCopy(gRend, gBGTex, NULL, NULL);

	
}

void initBGTex()
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
	temp = NULL;

	temp = SDL_LoadBMP("xybackground.bmp");

	rdst.x = 0;
	rdst.y = SCR_H / 2;
	rdst.h = SCR_H / 2;
	rdst.w = SCR_W;

	SDL_BlitScaled(temp, NULL, gSurf, &rdst);
	SDL_FreeSurface(temp);

	gBGTex = SDL_CreateTextureFromSurface(gRend, gSurf);
}

void Kollision(CannonBall* cBall, RopeBall* rBall)
{
	
	float Distance = length(cBall->pos - rBall->pos);

	if (Distance <= (cBall->radius + rBall->radius))
	{

		float e = 0.60f; //Coefficient of restitution
		float f = 0.16f; //Coefficient of friction
		float fr = 0.08f; //Coefficient of rollfriction

		vec3f ep = normalize(cBall->pos - rBall->pos);
		vec3f en = cross(normalize(cross((cBall->linVel - rBall->linVel), ep)), ep);

		vec3f er1 = ep;
		vec3f er2 = ep*(-1);

		//velocity before kollision against ep
		float v1p = dot(cBall->linVel, ep);
		float v2p = dot(rBall->linVel, ep);
		float sumMass = cBall->mass + rBall->mass;

		//Velocity after kollision
		float u1p = ((cBall->mass - rBall->mass*e) / sumMass)*v1p + (((1 - e)*rBall->mass) / sumMass)*v2p;
		float u2p = (((1 - e)*cBall->mass) / sumMass) *v1p + ((rBall->mass - cBall->mass*e) / sumMass)*v2p;
		
		vec3f cBallFinalU = cBall->linVel + (ep + en*f)*(u1p - v1p);
		vec3f rBallFinalU = rBall->linVel + (ep + en*f)*(u2p - v2p);
		
		vec3f cAngVel = cross(er1, en)*(f*(u1p - v1p) / (2 * cBall->radius));

		cBall->linVel = cBallFinalU;
		cBall->angVel = cAngVel;
		
		float betpos=((cBall->radius + rBall->radius)- (Distance));

		vec3f betPosV = vec3f(-betpos, -betpos, -betpos);
		cBall->pos = betPosV + cBall->pos;

		vec3f temp = normalize(rBall->pos - rBall->anchorpoint);	

		rBall->linVel = projectOnPlane(rBallFinalU,temp);

		vec3f rAngV = cross((rBall->linVel / rBall->radius), temp);
		rBall->angVel = rBall->angVel + rAngV;



	}

}




