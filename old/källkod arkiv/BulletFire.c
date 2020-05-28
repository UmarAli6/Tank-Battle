#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <time.h>
#define	MAX_BULLETS 1000
typedef struct
{
	float x, y;
	float dx;
	float dy;
	float angle;
	float rad;
	float vel;
	unsigned int bulletTimer;
	char *name;
} Man;

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
	int x, y;
} Tank;

typedef struct
{
	float x, y;
	float dx, dy;
	float angle;
}Bullet;

typedef struct
{
	//players
	Man man;
	//tanks
	Tank tanks[100];
	Bullet *bullets[MAX_BULLETS];
	//ledges
	LedgeSide ledges[100];

	//Images
	SDL_Texture *tank;
	SDL_Texture *brickSide;
	SDL_Texture *brickLong;
	SDL_Texture *bulletGreen;

	//Renderer
	SDL_Renderer *renderer;
} GameState;

void loadGame(GameState *game);
void collisionDetect(GameState *game);
int processEvents(SDL_Window *window, GameState *game);
void doRender(SDL_Renderer *renderer, GameState *game, Bullet *bullets[]);
void updateLogic(GameState *game);
void addBullet(Bullet *bullets[], Man man);
void removeBullet(Bullet *bullets[], int i);

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
		doRender(renderer, &gameState, gameState.bullets);

	}
	//Free malloc memory
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		removeBullet(gameState.bullets, i);
	}
	//Shutdown game and unload all memory
	SDL_DestroyTexture(gameState.tank);
	SDL_DestroyTexture(gameState.bulletGreen);

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
	surface = IMG_Load("assets/tank_green.png");
	if (surface == NULL)
	{
		printf("Could not find tank.png!\n");
		SDL_Quit();
		exit(1);
	}

	game->tank = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("assets/fenceRed.png");
	game->brickSide = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("assets/fenceRedlong.png");
	game->brickLong = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("assets/bulletDark2.png");
	game->bulletGreen = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	// Init tank members
	game->man.x = 320;
	game->man.y = 240;
	game->man.angle = 0.0;
	game->man.vel = 0.0;
	game->man.rad = 0.0;
	game->man.bulletTimer = 0;

	//Init bullet array to NULL
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		game->bullets[i] = NULL;
	}
	//init ledeges
	/*game->ledges[0].w = 100;
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
	game->ledges[7].y = 90;*/
}

void collisionDetect(GameState *game)
{
	//Ser till så att man håller sig innanför fönstret
	if (game->man.x <= 0) game->man.x = 0;
	if (game->man.y <= 0) game->man.y = 0;
	if (game->man.x >= 640 - 48) game->man.x = 640 - 48;
	if (game->man.y >= 480 - 48) game->man.y = 480 - 48;
	//Bullet bounds
	for (int i = 0; i < MAX_BULLETS; i++) if(game->bullets[i])
	{
		if (game->bullets[i]->x < -20 || game->bullets[i]->x > 700) removeBullet(game->bullets, i);
		else if (game->bullets[i]->y < -20 || game->bullets[i]->y > 700) removeBullet(game->bullets, i);
	}
	//Check for collision with any ledges (brick blocks)
	for (int i = 0; i < 100; i++)
	{
		float mw = 48, mh = 48;
		float mx = game->man.x, my = game->man.y;
		float bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;

		if (my + mh > by &&  my < by + bh)
		{
			//rubbing against right edge
			if (mx < bx + bw  &&  mx + mw > bx + bw)
			{
				//correct x
				game->man.x = bx + bw;
				mx = bx + bw;
			}
			//rubbing against left edge
			else if (mx + mw > bx && mx < bx)
			{
				//correct x
				game->man.x = bx - mw;
				mx = bx - mw;
			}
		}

		if (mx + mw > bx && mx < bx + bw)
		{
			//are we bumping our head
			if (my < by + bh && my > by)
			{
				//correct y
				game->man.y = by + bh;

			}
			//are we landing on the ledge
			else if (my + mh > by && my < by)
			{
				//correct y
				game->man.y = by - mh;
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
		game->man.angle -= 5.0;
		if (game->man.angle < 0.0) game->man.angle = 360.0;
	}
	if (state[SDL_SCANCODE_RIGHT])
	{
		game->man.angle += 5.0;
		if (game->man.angle > 359.0) game->man.angle = 0.0;
	}
	if (state[SDL_SCANCODE_SPACE])
	{
		currentTime = SDL_GetTicks();
		if (currentTime > game->man.bulletTimer +500) {
			addBullet(game->bullets, game->man);
			game->man.bulletTimer = currentTime;
		}
	}
	if (state[SDL_SCANCODE_UP])
	{
		game->man.vel = -5.0;
	}
	else if (state[SDL_SCANCODE_DOWN])
	{
		game->man.vel = 5.0;
	}
	else game->man.vel = 0.0;


	return done;
}

void updateLogic(GameState *game)
{
	game->man.rad = game->man.angle * (M_PI / 180.0);
	game->man.dx = cos(game->man.rad) * game->man.vel;
	game->man.dy = sin(game->man.rad) * game->man.vel;
	game->man.x += game->man.dx;
	game->man.y += game->man.dy;
	
	for (int i = 0; i < MAX_BULLETS; i++) if (game->bullets[i])
	{
		game->bullets[i]->x += game->bullets[i]->dx;
		game->bullets[i]->y += game->bullets[i]->dy;
	}
}

void addBullet(Bullet *bullets[], Man man)
{
	int found = -1;
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bullets[i] == NULL)
		{
			found = i;
			break;
		}
	}
	if (found >= 0)
	{
		bullets[found] = malloc(sizeof(Bullet));
		bullets[found]->angle = man.angle;
		bullets[found]->dx = cos(man.rad) * -8;
		bullets[found]->dy = sin(man.rad) * -8;
		bullets[found]->x = man.x;
		bullets[found]->y = man.y;
	}
	
}

void removeBullet(Bullet *bullets[], int i)
{
	if (bullets[i])
	{
		free(bullets[i]);
		bullets[i] = NULL;
	}
}

void doRender(SDL_Renderer *renderer, GameState *game, Bullet *bullets[])
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
	for (int i = 0; i < MAX_BULLETS; i++) if (bullets[i])
	{
		printf("%.1f\n,		[%.1f, %.1f]\n", bullets[i]->angle, bullets[i]->x, bullets[i]->y);
		SDL_Rect bullrect = { bullets[i]->x+20, bullets[i]->y+20, 10, 10 };
		SDL_RenderCopyEx(renderer, game->bulletGreen, NULL, &bullrect, bullets[i]->angle + 90, NULL, SDL_FLIP_VERTICAL);
	}

	SDL_Rect rect = { game->man.x, game->man.y,48,48 };
	SDL_RenderCopyEx(renderer, game->tank, NULL, &rect, game->man.angle + 90, NULL, 0);

	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_RenderPresent(renderer);

}
