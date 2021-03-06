#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <time.h>
#include <tank.h>
#include <bullet.h>

#define MAX_BULLETS 1000

typedef struct
{
	int x, y, w, h;
} LedgeSide;

typedef struct
{
	int x, y, w, h;
} LedgeLong;

typedef struct
{
	//Bullet
	Bullet bullets[MAX_BULLETS];

	//Tanks
	Tank t1;

	//ledges
	LedgeSide ledges[100];

	//Images
	SDL_Texture *bild;
	SDL_Texture *brickSide;
	SDL_Texture *brickLong;
	SDL_Texture *bulletGreen;

	//Renderer
	SDL_Renderer *renderer;
} GameState;

void loadGame(GameState *game);
void collisionDetect(GameState *game);
int processEvents(SDL_Window *window, GameState *game);
void doRender(SDL_Renderer *renderer, GameState *game);
void updateLogic(GameState *game);

int main(int argc, char *argv[])
{
	GameState gameState;
	SDL_Window *window;                    // Declare a window
	SDL_Renderer *renderer;                // Declare a renderer
	SDL_Surface *tankSurface = NULL;

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

										   //Create an application window with the following settings:
	window = SDL_CreateWindow("Game Window",                     // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		640,                               // width, in pixels
		480,                               // height, in pixels
		0                                  // flags
	);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	gameState.renderer = renderer;

	loadGame(&gameState);

	// The window is open: enter program loop (see SDL_PollEvent)
	int done = 0;

	//Event loop
	while (!done)
	{
		//Check for events
		done = processEvents(window, &gameState);

		// update entity members
		updateLogic(&gameState);

		collisionDetect(&gameState);

		//Render display
		doRender(renderer, &gameState);

	}

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (gameState.bullets[i] != NULL)
		{
			destroyBullet(gameState.bullets[i],i);
			gameState.bullets[i] = NULL;
		}
	}

	destroyTank(gameState.t1);
	//Shutdown game and unload all memory
	SDL_DestroyTexture(gameState.bild);

	// Close and destroy the window
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	// Clean up
	SDL_Quit();
	return 0;
}

void loadGame(GameState *game)
{
	SDL_Surface *surface = NULL;
	//Load images and create rendering textures from them
	surface = IMG_Load("tank_sand.png");
	if (surface == NULL)
	{
		printf("Could not find tank.png!\n");
		SDL_Quit();
		exit(1);
	}

	game->bild = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("fenceRed.png");
	game->brickSide = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("fenceRedlong.png");
	game->brickLong = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("bulletDark2.png");
	game->bulletGreen = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	game->t1 = createTank(340,240);

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		game->bullets[i] = NULL;
	}

	//init ledeges
	game->ledges[0].w = 100;
	game->ledges[0].h = 20;
	game->ledges[0].x = 400;
	game->ledges[0].y = 85;

	game->ledges[49].w = 20;
	game->ledges[49].h = 90;
	game->ledges[49].x = 250;
	game->ledges[49].y = 0;

	game->ledges[50].w = 20;
	game->ledges[50].h = 90;
	game->ledges[50].x = 490;
	game->ledges[50].y = 103;

	game->ledges[1].w = 100;
	game->ledges[1].h = 20;
	game->ledges[1].x = 580;
	game->ledges[1].y = 103;

	game->ledges[51].w = 20;
	game->ledges[51].h = 90;
	game->ledges[51].x = 490;
	game->ledges[51].y = 280;

	game->ledges[2].w = 100;
	game->ledges[2].h = 20;
	game->ledges[2].x = 395;
	game->ledges[2].y = 363;

	game->ledges[3].w = 100;
	game->ledges[3].h = 20;
	game->ledges[3].x = 260;
	game->ledges[3].y = 260;

	game->ledges[4].w = 100;
	game->ledges[4].h = 20;
	game->ledges[4].x = 260;
	game->ledges[4].y = 170;

	game->ledges[52].w = 20;
	game->ledges[52].h = 90;
	game->ledges[52].x = 245;
	game->ledges[52].y = 180;

	game->ledges[53].w = 20;
	game->ledges[53].h = 90;
	game->ledges[53].x = 355;
	game->ledges[53].y = 180;

	game->ledges[5].w = 100;
	game->ledges[5].h = 20;
	game->ledges[5].x = 120;
	game->ledges[5].y = 363;

	game->ledges[6].w = 100;
	game->ledges[6].h = 20;
	game->ledges[6].x = 0;
	game->ledges[6].y = 170;

	game->ledges[54].w = 20;
	game->ledges[54].h = 90;
	game->ledges[54].x = 90;
	game->ledges[54].y = 180;

	game->ledges[7].w = 100;
	game->ledges[7].h = 20;
	game->ledges[7].x = 160;
	game->ledges[7].y = 90;
}

void collisionDetect(GameState *game)
{
	//Ser till så att man håller sig innanför fönstret
	if (getTankPositionX(game->t1) <= 0)
	{
		setTankX(game->t1, 0);
	}
	if (getTankPositionY(game->t1) <= 0)
	{
		setTankY(game->t1, 0);
	}
	if (getTankPositionX(game->t1) >= 640 - 48)
	{
		setTankX(game->t1, 592);
	}
	if (getTankPositionY(game->t1) >= 480 - 48)
	{
		setTankY(game->t1, 432);
	}

	//Bullets within window
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (game->bullets[i]!=NULL)
		{
			{
				if (getBulletPositionX(game->bullets[i]) < -20 || getBulletPositionX(game->bullets[i]) > 700)
				{
					destroyBullet(game->bullets[i],i);
					game->bullets[i] = NULL;
				}
				else if (getBulletPositionY(game->bullets[i]) < -20 || getBulletPositionY(game->bullets[i]) > 700)
				{
					destroyBullet(game->bullets[i],i);
					game->bullets[i] = NULL;
				}
			}
		}
	}

	//Check for collision with any ledges (brick blocks)
	for (int i = 0; i < 100; i++)
	{
		int mw = 48, mh = 48;
		int mx = getTankPositionX(game->t1), my = getTankPositionY(game->t1);
		int bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;


		if (my + mh > by &&  my < by + bh)
		{
			//rubbing against right edge
			if (mx < bx + bw  &&  mx + mw > bx + bw)
			{
				//correct x
				setTankX(game->t1, bx + bw);
				mx = bx + bw;
			}
			//rubbing against left edge
			else if (mx + mw > bx && mx < bx)
			{
				//correct x
				setTankX(game->t1, bx - mw);
				mx = bx - mw;
			}
		}

		if (mx + mh > bx && mx < bx + bw)
		{
			//are we bumping our head
			if (my < by + bh && my > by)
			{
				//correct y
				setTankY(game->t1, by + bh);

			}
			//are we landing on the ledge
			else if (my + mh > by && my < by)
			{
				//correct y
				setTankY(game->t1, by - mh);
			}
		}
	}
}

int processEvents(SDL_Window *window, GameState *game)
{
	SDL_Event event;
	int done = 0;
	unsigned int currentTime;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (window)
			{
				SDL_DestroyWindow(window);
				window = NULL;
				done = 1;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				done = 1;
				break;
			}
		}
		break;
		case SDL_QUIT:
			//quit out of the game
			done = 1;
			break;
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT])
	{
		updateTankAngle(game->t1, -1);
		if (getTankAngle(game->t1) < 0)
		{
			setTankAngle(game->t1, 360);
		}
	}
	if (state[SDL_SCANCODE_RIGHT])
	{
		updateTankAngle(game->t1, 1);
		if (getTankAngle(game->t1) > 359)
		{
			setTankAngle(game->t1, 0);
		}
	}
	if (state[SDL_SCANCODE_SPACE])
	{
		int found = -1;
		currentTime = SDL_GetTicks();
		if (currentTime > getTimer(game->t1) + 300)
		{
			for (int i = 0; i < MAX_BULLETS; i++)
			{
				if (game->bullets[i] == NULL);
				{
					found = i;
					break;
				}
			}
			if (found >= 0)
			{
				game->bullets[found] = createBullet(getTankPositionX(game->t1), getTankPositionY(game->t1), sin(getTankRadian(game->t1)), cos(getTankRadian(game->t1)), getTankAngle(game->t1),found);
				setTimer(game->t1, currentTime);
			}
		}
	}
	if (state[SDL_SCANCODE_UP])
	{
		setTankVelocity(game->t1, -1);
	}
	else if (state[SDL_SCANCODE_DOWN])
	{
		setTankVelocity(game->t1, 1);
	}
	else
	{
		setTankVelocity(game->t1, 0);
	}

	return done;
}

void updateLogic(GameState *game)
{
	setTankRadian(game->t1, getTankAngle(game->t1) * (M_PI / 180.0));
	setTankDx(game->t1, cos(getTankRadian(game->t1)) * getTankVelocity(game->t1));
	setTankDy(game->t1, sin(getTankRadian(game->t1)) * getTankVelocity(game->t1));
	updateTankPositionX(game->t1, getTankDx(game->t1));
	updateTankPositionY(game->t1, getTankDy(game->t1));

	for (int i = 0; i < MAX_BULLETS; i++) if (game->bullets[i])
	{
		updateBulletPositionX(game->bullets[i], getBulletDx(game->bullets[i]));
		updateBulletPositionY(game->bullets[i], getBulletDy(game->bullets[i]));
	}
}

void doRender(SDL_Renderer *renderer, GameState *game)
{
	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int i = 0; i<49; i++)
	{
		SDL_Rect ledgeRect = { game->ledges[i].x,game->ledges[i].y,game->ledges[i].w,game->ledges[i].h };
		SDL_RenderCopy(renderer, game->brickSide, NULL, &ledgeRect);
	}
	for (int i = 49; i<100; i++)
	{
		SDL_Rect ledgeRect = { game->ledges[i].x,game->ledges[i].y,game->ledges[i].w,game->ledges[i].h };
		SDL_RenderCopy(renderer, game->brickLong, NULL, &ledgeRect);
	}

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (game->bullets[i]!=NULL)
		{
			printf("%.1f\n,		[%.1f, %.1f, nr:%d]\n", getBulletAngle(game->bullets[i]), getBulletPositionX(game->bullets[i]), getBulletPositionY(game->bullets[i]), i); 
			SDL_Rect bullrect = { getBulletPositionX(game->bullets[i]) + 20, getBulletPositionY(game->bullets[i]) + 24, 10, 10 };
			SDL_RenderCopyEx(renderer, game->bulletGreen, NULL, &bullrect, getBulletAngle(game->bullets[i]) + 90, NULL, SDL_FLIP_VERTICAL);
		}
	}

	SDL_Rect rect = { getTankPositionX(game->t1), getTankPositionY(game->t1),48,48 };
	SDL_RenderCopyEx(renderer, game->bild, NULL, &rect, getTankAngle(game->t1)+90, NULL, 0);



	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_RenderPresent(renderer);
}
