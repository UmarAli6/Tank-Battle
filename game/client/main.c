#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_ttf.h>
#include "tank.h"
#include "bullet.h"
#include "game.h"
#include "network.h"

#define WIDTH 960
#define HEIGTH 720

int main(int argc, char *argv[])
{
	UDPsocket sd;
	IPaddress srvadd;
	UDPpacket *p;
	UDPpacket *p2;

	int nrOfConnections = 0;

	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		getchar();
		exit(EXIT_FAILURE);
	}

	if (!(sd = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		getchar();
		exit(EXIT_FAILURE);
	}

	if (!((p = SDLNet_AllocPacket(512)) && (p2 = SDLNet_AllocPacket(512))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		getchar();
		exit(EXIT_FAILURE);
	}

	if (TTF_Init() < 0) {
		fprintf(stderr, "TTF_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	
	
	GameState gameState;
	SDL_Window *window;                    // Declare a window
	SDL_Renderer *renderer;                // Declare a renderer
	
	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

										   //Create an application window with the following settings:
	window = SDL_CreateWindow("Tank Battle - connect using the terminal", // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		WIDTH,                               // width, in pixels
		HEIGTH,                               // height, in pixels
		0                                  // flags
	);
	int which;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	gameState.renderer = renderer;
	gameState.font = TTF_OpenFont("resources/arial.ttf", 45);
	if (!gameState.font) {
		printf("Could not load font!\n");
		getchar();
		SDL_Quit();
		exit(1);
	}

	int gameOver = 0, done = 0;
	while (!done)
	{
		if (showMenu(&gameState)) done = !done;
		while (!setupConnection(&sd, &srvadd, p, p2, &nrOfConnections)) {}
		loadGame(&gameState, &nrOfConnections, &sd, &srvadd, p, p2, &which, window); // The window is open: enter Game loop (see SDL_PollEvent)

		//Game loop
		while (!gameOver)
		{
			//Check for events
			gameOver = done = processEvents(window, &gameState, &sd, &srvadd, p, &nrOfConnections);

			//Update entity members
			updateLogic(&gameState, &sd, &srvadd, p);

			//Check for Collision
			collisionDetect(&gameState);

			//Render display
			doRender(renderer, &gameState, &nrOfConnections, &sd, p2, &which);

			//Check if game is done 
			handleScore(&gameState, &gameOver, window);
		}
		resetGame(&gameState, &sd, &srvadd, p, &nrOfConnections, &gameOver, window);
	}


	//Shutdown game and unload all memory
	SDL_DestroyTexture(gameState.waitFlag);
	for (int j = 0; j < 3; j++)
	{
		SDL_DestroyTexture(gameState.label[j]);
	}
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	// Clean up
	TTF_Quit();
	SDL_Quit();
	return 0;
}
