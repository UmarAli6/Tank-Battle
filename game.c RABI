#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "network.h"
#include <string.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include "SDL_image.h"
#include "SDL.h"

#define WIDTH 960
#define HEIGHT 720
#define MAX_LEDGES 50

void loadGame(GameState* game, int *pnrOfConnections, UDPsocket *sd, IPaddress *srvadd, UDPpacket *p, UDPpacket *p2, int *pWhich) {
	SDL_Surface *surface = NULL;
	//Load images and create rendering textures from them
	surface = IMG_Load("resources/tank_sand.png");
	(*game).tankSandTexture = SDL_CreateTextureFromSurface((*game).renderer, surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("resources/tank_sand_dead.png");
	(*game).tankSandTextureDead = SDL_CreateTextureFromSurface((*game).renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("resources/tank_green.png");
	(*game).tankGreenTexture = SDL_CreateTextureFromSurface((*game).renderer, surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("resources/tank_green_dead.png");
	(*game).tankGreenTextureDead = SDL_CreateTextureFromSurface((*game).renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("resources/tank_dark.png");
	(*game).tankDarkTexture = SDL_CreateTextureFromSurface((*game).renderer, surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("resources/tank_dark_dead.png");
	(*game).tankDarkTextureDead = SDL_CreateTextureFromSurface((*game).renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("resources/tank_blue.png");
	(*game).tankBlueTexture = SDL_CreateTextureFromSurface((*game).renderer, surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("resources/tank_blue_dead.png");
	(*game).tankBlueTextureDead = SDL_CreateTextureFromSurface((*game).renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("resources/bullet.png");
	(*game).bullet = SDL_CreateTextureFromSurface((*game).renderer, surface);
	SDL_FreeSurface(surface);

	if (*pnrOfConnections == 1) {
		(*game).tanks[0] = createTank(10, 10);
		setTankAngle((*game).tanks[0], -90);
		(*game).tanks[1] = createTank(10, 660);
		setTankAngle((*game).tanks[1], 90);
		(*game).tanks[2] = createTank(880, 10);
		setTankAngle((*game).tanks[2], -90);
		(*game).tanks[3] = createTank(880, 660);
		setTankAngle((*game).tanks[3], 90);
		*pWhich = 1;
	}
	if (*pnrOfConnections == 2) {

		(*game).tanks[1] = createTank(10, 10);
		setTankAngle((*game).tanks[1], -90);
		(*game).tanks[0] = createTank(880, 10);
		setTankAngle((*game).tanks[0], -90);
		(*game).tanks[2] = createTank(880, 660);
		setTankAngle((*game).tanks[2], 90);
		(*game).tanks[3] = createTank(10, 660);
		setTankAngle((*game).tanks[3], 90);
		*pWhich = 2;
	}
	if (*pnrOfConnections == 3) {

		(*game).tanks[1] = createTank(880, 660);
		setTankAngle((*game).tanks[1], 90);
		(*game).tanks[2] = createTank(10, 10);
		setTankAngle((*game).tanks[2], -90);
		(*game).tanks[0] = createTank(10, 660);
		setTankAngle((*game).tanks[0], 90);
		(*game).tanks[3] = createTank(880, 10);
		setTankAngle((*game).tanks[3], -90);
		*pWhich = 3;
	}
	if (*pnrOfConnections == 4) {

		(*game).tanks[1] = createTank(880, 10);
		setTankAngle((*game).tanks[1], -90);
		(*game).tanks[2] = createTank(10, 660);
		setTankAngle((*game).tanks[2], 90);
		(*game).tanks[3] = createTank(10, 10);
		setTankAngle((*game).tanks[3], -90);
		(*game).tanks[0] = createTank(880, 660);
		setTankAngle((*game).tanks[0], 90);
		*pWhich = 4;
	}


	for (int i = 0; i < MAX_BULLETS; i++)
	{
		(*game).bullets[i] = NULL;
	}

	//init ledeges
	(*game).ledges[0].w = WIDTH;
	(*game).ledges[0].h = 5;
	(*game).ledges[0].x = 0;
	(*game).ledges[0].y = 0;

	(*game).ledges[1].w = 5;
	(*game).ledges[1].h = HEIGHT;
	(*game).ledges[1].x = 0;
	(*game).ledges[1].y = 0;

	(*game).ledges[2].w = WIDTH;
	(*game).ledges[2].h = 5;
	(*game).ledges[2].x = 0;
	(*game).ledges[2].y = HEIGHT - 5;

	(*game).ledges[3].w = 5;
	(*game).ledges[3].h = HEIGHT;
	(*game).ledges[3].x = WIDTH - 5;
	(*game).ledges[3].y = 0;

	(*game).ledges[4].w = 5;
	(*game).ledges[4].h = 120;
	(*game).ledges[4].x = 480;
	(*game).ledges[4].y = 0;

	(*game).ledges[5].w = 120;
	(*game).ledges[5].h = 5;
	(*game).ledges[5].x = 480;
	(*game).ledges[5].y = 120;

	(*game).ledges[6].w = 240;
	(*game).ledges[6].h = 5;
	(*game).ledges[6].x = 120;
	(*game).ledges[6].y = 120;

	(*game).ledges[7].w = 5;
	(*game).ledges[7].h = 120;
	(*game).ledges[7].x = 360;
	(*game).ledges[7].y = 120;

	(*game).ledges[8].w = 5;
	(*game).ledges[8].h = 120;
	(*game).ledges[8].x = 120;
	(*game).ledges[8].y = 240;

	(*game).ledges[9].w = 240;
	(*game).ledges[9].h = 5;
	(*game).ledges[9].x = 120;
	(*game).ledges[9].y = 360;

	(*game).ledges[10].w = 5;
	(*game).ledges[10].h = 240;
	(*game).ledges[10].x = 360;
	(*game).ledges[10].y = 360;

	(*game).ledges[11].w = 240;
	(*game).ledges[11].h = 5;
	(*game).ledges[11].x = 0;
	(*game).ledges[11].y = 480;

	(*game).ledges[12].w = 5;
	(*game).ledges[12].h = 120;
	(*game).ledges[12].x = 840;
	(*game).ledges[12].y = 600;

	(*game).ledges[13].w = 5;
	(*game).ledges[13].h = 120;
	(*game).ledges[13].x = 600;
	(*game).ledges[13].y = 600;

	(*game).ledges[14].w = 120;
	(*game).ledges[14].h = 5;
	(*game).ledges[14].x = 120;
	(*game).ledges[14].y = 600;

	(*game).ledges[15].w = 5;
	(*game).ledges[15].h = 120;
	(*game).ledges[15].x = 480;
	(*game).ledges[15].y = 240;

	(*game).ledges[16].w = 120;
	(*game).ledges[16].h = 5;
	(*game).ledges[16].x = 600;
	(*game).ledges[16].y = 240;

	(*game).ledges[17].w = 120;
	(*game).ledges[17].h = 5;
	(*game).ledges[17].x = 720;
	(*game).ledges[17].y = 120;

	(*game).ledges[18].w = 5;
	(*game).ledges[18].h = 240;
	(*game).ledges[18].x = 720;
	(*game).ledges[18].y = 120;

	(*game).ledges[19].w = 120;
	(*game).ledges[19].h = 5;
	(*game).ledges[19].x = 840;
	(*game).ledges[19].y = 240;

	(*game).ledges[20].w = 120;
	(*game).ledges[20].h = 5;
	(*game).ledges[20].x = 720;
	(*game).ledges[20].y = 360;

	(*game).ledges[21].w = 240;
	(*game).ledges[21].h = 5;
	(*game).ledges[21].x = 480;
	(*game).ledges[21].y = 480;

	(*game).ledges[22].w = 5;
	(*game).ledges[22].h = 120;
	(*game).ledges[22].x = 840;
	(*game).ledges[22].y = 360;

	(*game).ledges[23].w = 5;
	(*game).ledges[23].h = 120;
	(*game).ledges[23].x = 720;
	(*game).ledges[23].y = 480;
	(*game).haveUpdated = 1;
}

void collisionDetect(GameState* game) {
	//The sides of tank and wall
	int tankLeft, wallLeft;
	int tankRight, wallRight;
	int tankTop, wallTop;
	int tankBottom, wallBottom;

	//Check for collision with any ledges and frame
	for (int i = 0; i < MAX_LEDGES; i++)
	{
		//Calculate the sides of the tank
		tankLeft = getTankPositionX((*game).tanks[0]);
		tankRight = getTankPositionX((*game).tanks[0]) + 48;
		tankTop = getTankPositionY((*game).tanks[0]);
		tankBottom = getTankPositionY((*game).tanks[0]) + 48;

		//Calculate the sides of the wall
		wallLeft = game->ledges[i].x;
		wallRight = game->ledges[i].x + game->ledges[i].w;
		wallTop = game->ledges[i].y;
		wallBottom = game->ledges[i].y + game->ledges[i].h;


		//If all side of the tank is outside of the wall, if not -> collision
		if (tankBottom >= wallTop && tankTop <= wallBottom && tankRight >= wallLeft && tankLeft <= wallRight)
		{
			setTankY((*game).tanks[0], (getTankPositionY((*game).tanks[0])) - getTankDy((*game).tanks[0]));
			setTankX((*game).tanks[0], (getTankPositionX((*game).tanks[0])) - getTankDx((*game).tanks[0]));
		}
	}

	//Tar bort skott som träffar och dödar andra spelare eller åker utanför banan
	int bDestroyed;
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bDestroyed = 0;
		if ((*game).bullets[i] != NULL)
		{
			for (int j = 0; j < 4 && (*game).bullets[i] != NULL; j++)
			{
				if (getBulletPositionX((*game).bullets[i])    > getTankPositionX((*game).tanks[j]) - 24 && getBulletPositionX((*game).bullets[i]) < getTankPositionX((*game).tanks[j]) + 24 && getBulletPositionY((*game).bullets[i]) > getTankPositionY((*game).tanks[j]) - 24 && getBulletPositionY((*game).bullets[i]) < getTankPositionY((*game).tanks[j]) + 24) {
					if (bulletOwner((*game).bullets[i]) != j) {
						setHealth((*game).tanks[j], 0);
						destroyBullet(game->bullets, i);
						bDestroyed = 1;
					}
				}
			}

			if (!bDestroyed && (*game).bullets[i] != NULL) {
				if (getBulletPositionX((*game).bullets[i]) < 0 || getBulletPositionX((*game).bullets[i]) > 960)
				{
					destroyBullet(&(*game).bullets, i);
					bDestroyed = 1;
				}
				else if (getBulletPositionY((*game).bullets[i]) < 0 || getBulletPositionY((*game).bullets[i]) > 720)
				{
					destroyBullet(&(*game).bullets, i);
					bDestroyed = 1;
				}
			}

			for (int u = 0; u < 24 && bDestroyed == 0; u++)
			{
				if (getBulletPositionX((*game).bullets[i]) >(*game).ledges[u].x - 24 && getBulletPositionX((*game).bullets[i]) < ((*game).ledges[u].x + (*game).ledges[u].w - 24) && getBulletPositionY((*game).bullets[i]) > (*game).ledges[u].y - 24 && getBulletPositionY((*game).bullets[i]) < ((*game).ledges[u].y + (*game).ledges[u].h - 24)) {
					destroyBullet(game->bullets, i);
					bDestroyed = 1;
				}
			}

			//Laddar om så att spelaren kan skjuta igen då spelaren ändast kan ha 2 skott ute samtidgt
			if (bDestroyed) {
				switch (i)
				{
				case 0:
					resetAmmo((*game).tanks[0], i % 2);
					break;
				case 1:
					resetAmmo((*game).tanks[0], i % 2);
					break;
				case 2:
					resetAmmo((*game).tanks[1], i % 2);
					break;
				case 3:
					resetAmmo((*game).tanks[1], i % 2);
					break;
				case 4:
					resetAmmo((*game).tanks[2], i % 2);
					break;
				case 5:
					resetAmmo((*game).tanks[2], i % 2);
					break;
				case 6:
					resetAmmo((*game).tanks[3], i % 2);
					break;
				case 7:
					resetAmmo((*game).tanks[3], i % 2);
					break;
				default:
					break;
				}
			}
		}
	}
}

int processEvents(SDL_Window *window, GameState *game, UDPsocket *sd, IPaddress *srvadd, UDPpacket *p, int *pNrOfConnections) {
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
	if (state[SDL_SCANCODE_LEFT] && getHealth((*game).tanks[0])) //Kolla också hp så den inte är död för att dem inte ska kunna röra sig när dem dör.
	{
		setTankAngleOLD((*game).tanks[0], getTankAngle((*game).tanks[0]));
		updateTankAngle((*game).tanks[0], -3);
		if (getTankAngle((*game).tanks[0]) < 0)
		{
			setTankAngle((*game).tanks[0], 360);
		}
	}
	if (state[SDL_SCANCODE_RIGHT] && getHealth((*game).tanks[0]))
	{
		setTankAngleOLD((*game).tanks[0], getTankAngle((*game).tanks[0]));
		updateTankAngle((*game).tanks[0], 3);
		if (getTankAngle((*game).tanks[0]) > 359)
		{
			setTankAngle((*game).tanks[0], 0);
		}
	}
	if (state[SDL_SCANCODE_SPACE] && getHealth((*game).tanks[0]) && *pNrOfConnections == 4) //Lever och försöker skjuta och att alla är anslutna
	{
		int returnedNumber;
		currentTime = SDL_GetTicks();
		if (currentTime > getTimer((*game).tanks[0]) + 500)
		{
			if (getAmmo((*game).tanks[0], &returnedNumber)) {
				//Här för att skicka så den vet att ett skott har skjuttit 555 för skott just nu
				float xNEW = 5.0f, yNEW = 5.0f, angleNEW = 5.0f;
				sprintf((char *)p->data, "%f %f %f\n", xNEW, yNEW, angleNEW);
				p->address.host = (*srvadd).host;    /* Set the destination host */
				p->address.port = (*srvadd).port;    /* And destination port */
				p->len = strlen((char *)p->data) + 1;
				SDLNet_UDP_Send(*sd, -1, p);

				(*game).bullets[returnedNumber] = createBullet(getTankPositionX((*game).tanks[0]), getTankPositionY((*game).tanks[0]), sin(getTankRadian((*game).tanks[0])), cos(getTankRadian((*game).tanks[0])), getTankAngle((*game).tanks[0]), 0);
				setTimer((*game).tanks[0], currentTime);
			}
		}
	}
	if (state[SDL_SCANCODE_UP] && getHealth((*game).tanks[0]))
	{
		setTankVelocity((*game).tanks[0], -3);
	}
	else if (state[SDL_SCANCODE_DOWN] && getHealth((*game).tanks[0]))
	{
		setTankVelocity((*game).tanks[0], 3);
	}
	else
	{
		setTankVelocity((*game).tanks[0], 0);
	}

	return done;
}

void doRender(SDL_Renderer *renderer, GameState *game, int *pnrOfConnections, UDPsocket *sd, UDPpacket *p2, int *pWhich) {
	float xCORD, yCORD, dAngle;
	int c, returnedNumber;
	if (SDLNet_UDP_Recv(*sd, p2)) {
		sscanf((char *)p2->data, "%f %f %d %f %d\n", &xCORD, &yCORD, &c, &dAngle, pnrOfConnections);
		if (c == 1) {
			if (xCORD == 6 && yCORD == 6 && dAngle == 6) {
				if (getAmmo((*game).tanks[1], &returnedNumber)) {
					setTankRadian((*game).tanks[1], getTankAngle((*game).tanks[1]) * (M_PI / 180.0)); //Måste sätta tank radian här då det inte görs för de andra tankarna som skickas över nätverket
					(*game).bullets[c * 2 + returnedNumber] = createBullet(getTankPositionX((*game).tanks[1]), getTankPositionY((*game).tanks[1]), sin(getTankRadian((*game).tanks[1])), cos(getTankRadian((*game).tanks[1])), getTankAngle((*game).tanks[1]), c);
					setTimer((*game).tanks[1], SDL_GetTicks());
				}
			}
			else {
				setEnemyX((*game).tanks[1], xCORD);
				setEnemyY((*game).tanks[1], yCORD);
				setEnemyAngle((*game).tanks[1], dAngle);
			}
		}
		if (c == 2) {
			if (xCORD == 6 && yCORD == 6 && dAngle == 6) {
				if (getAmmo((*game).tanks[2], &returnedNumber)) {
					setTankRadian((*game).tanks[2], getTankAngle((*game).tanks[2]) * (M_PI / 180.0));
					(*game).bullets[c * 2 + returnedNumber] = createBullet(getTankPositionX((*game).tanks[2]), getTankPositionY((*game).tanks[2]), sin(getTankRadian((*game).tanks[2])), cos(getTankRadian((*game).tanks[2])), getTankAngle((*game).tanks[2]), c);
					setTimer((*game).tanks[2], SDL_GetTicks());
				}
			}
			else {
				setEnemyX((*game).tanks[2], xCORD);
				setEnemyY((*game).tanks[2], yCORD);
				setEnemyAngle((*game).tanks[2], dAngle);
			}
		}
		if (c == 3) {
			if (xCORD == 6 && yCORD == 6 && dAngle == 6) {
				if (getAmmo((*game).tanks[3], &returnedNumber)) {
					setTankRadian((*game).tanks[3], getTankAngle((*game).tanks[3]) * (M_PI / 180.0));
					(*game).bullets[c * 2 + returnedNumber] = createBullet(getTankPositionX((*game).tanks[3]), getTankPositionY((*game).tanks[3]), sin(getTankRadian((*game).tanks[3])), cos(getTankRadian((*game).tanks[3])), getTankAngle((*game).tanks[3]), c);
					setTimer((*game).tanks[3], SDL_GetTicks());
				}
			}
			else {
				setEnemyX((*game).tanks[3], xCORD);
				setEnemyY((*game).tanks[3], yCORD);
				setEnemyAngle((*game).tanks[3], dAngle);
			}
		}
	}
	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_SetRenderDrawColor(renderer, 192, 192, 192, 0);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//set the drawing color to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);


	//Ritar väggarna i spelbanan
	for (int i = 0; i<MAX_LEDGES; i++)
	{
		SDL_Rect ledgeRect = { game->ledges[i].x,game->ledges[i].y,game->ledges[i].w,game->ledges[i].h };
		SDL_RenderFillRect(renderer, &ledgeRect);

	}

	//Ritar skottens positioner
	for (int i = 0; i < MAX_BULLETS; i++) if ((*game).bullets[i])
	{
		SDL_Rect bullrect = { getBulletPositionX((*game).bullets[i]) + 20, getBulletPositionY((*game).bullets[i]) + 24, 10, 10 };
		SDL_RenderCopyEx(renderer, (*game).bullet, NULL, &bullrect, getBulletAngle((*game).bullets[i]) + 90, NULL, SDL_FLIP_VERTICAL);
	}
	SDL_Rect rect = { getTankPositionX((*game).tanks[0]), getTankPositionY((*game).tanks[0]),48,48 };
	SDL_Rect secondDest = { getTankPositionX((*game).tanks[1]), getTankPositionY((*game).tanks[1]) ,48,48 };
	SDL_Rect thirdDest = { getTankPositionX((*game).tanks[2]), getTankPositionY((*game).tanks[2]) ,48,48 };
	SDL_Rect fourthDest = { getTankPositionX((*game).tanks[3]), getTankPositionY((*game).tanks[3]) ,48,48 };

	switch (*pWhich) //Kolla vilken playerIndex man är för att bestämma färgerna på tankarna rätt, men även ifall dem är döda
	{
	case 1:
		if (getHealth((*game).tanks[0])) SDL_RenderCopyEx(renderer, (*game).tankSandTexture, NULL, &rect, getTankAngle((*game).tanks[0]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankSandTextureDead, NULL, &rect, getTankAngle((*game).tanks[0]) + 90, NULL, 0);
		if (getHealth((*game).tanks[1])) SDL_RenderCopyEx(renderer, (*game).tankDarkTexture, NULL, &secondDest, getTankAngle((*game).tanks[1]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankDarkTextureDead, NULL, &secondDest, getTankAngle((*game).tanks[1]) + 90, NULL, 0);
		if (getHealth((*game).tanks[2])) SDL_RenderCopyEx(renderer, (*game).tankGreenTexture, NULL, &thirdDest, getTankAngle((*game).tanks[2]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankGreenTextureDead, NULL, &thirdDest, getTankAngle((*game).tanks[2]) + 90, NULL, 0);
		if (getHealth((*game).tanks[3])) SDL_RenderCopyEx(renderer, (*game).tankBlueTexture, NULL, &fourthDest, getTankAngle((*game).tanks[3]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankBlueTextureDead, NULL, &fourthDest, getTankAngle((*game).tanks[3]) + 90, NULL, 0);
		break;
	case 2:
		if (getHealth((*game).tanks[0])) SDL_RenderCopyEx(renderer, (*game).tankGreenTexture, NULL, &rect, getTankAngle((*game).tanks[0]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankGreenTextureDead, NULL, &rect, getTankAngle((*game).tanks[0]) + 90, NULL, 0);
		if (getHealth((*game).tanks[1])) SDL_RenderCopyEx(renderer, (*game).tankSandTexture, NULL, &secondDest, getTankAngle((*game).tanks[1]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankSandTextureDead, NULL, &secondDest, getTankAngle((*game).tanks[1]) + 90, NULL, 0);
		if (getHealth((*game).tanks[2])) SDL_RenderCopyEx(renderer, (*game).tankBlueTexture, NULL, &thirdDest, getTankAngle((*game).tanks[2]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankBlueTextureDead, NULL, &thirdDest, getTankAngle((*game).tanks[2]) + 90, NULL, 0);
		if (getHealth((*game).tanks[3])) SDL_RenderCopyEx(renderer, (*game).tankDarkTexture, NULL, &fourthDest, getTankAngle((*game).tanks[3]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankDarkTextureDead, NULL, &fourthDest, getTankAngle((*game).tanks[3]) + 90, NULL, 0);
		break;
	case 3:
		if (getHealth((*game).tanks[0])) SDL_RenderCopyEx(renderer, (*game).tankDarkTexture, NULL, &rect, getTankAngle((*game).tanks[0]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankDarkTextureDead, NULL, &rect, getTankAngle((*game).tanks[0]) + 90, NULL, 0);
		if (getHealth((*game).tanks[1])) SDL_RenderCopyEx(renderer, (*game).tankBlueTexture, NULL, &secondDest, getTankAngle((*game).tanks[1]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankBlueTextureDead, NULL, &secondDest, getTankAngle((*game).tanks[1]) + 90, NULL, 0);
		if (getHealth((*game).tanks[2])) SDL_RenderCopyEx(renderer, (*game).tankSandTexture, NULL, &thirdDest, getTankAngle((*game).tanks[2]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankSandTextureDead, NULL, &thirdDest, getTankAngle((*game).tanks[2]) + 90, NULL, 0);
		if (getHealth((*game).tanks[3])) SDL_RenderCopyEx(renderer, (*game).tankGreenTexture, NULL, &fourthDest, getTankAngle((*game).tanks[3]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankGreenTextureDead, NULL, &fourthDest, getTankAngle((*game).tanks[3]) + 90, NULL, 0);
		break;
	case 4:
		if (getHealth((*game).tanks[0])) SDL_RenderCopyEx(renderer, (*game).tankBlueTexture, NULL, &rect, getTankAngle((*game).tanks[0]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankBlueTextureDead, NULL, &rect, getTankAngle((*game).tanks[0]) + 90, NULL, 0);
		if (getHealth((*game).tanks[1])) SDL_RenderCopyEx(renderer, (*game).tankGreenTexture, NULL, &secondDest, getTankAngle((*game).tanks[1]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankGreenTextureDead, NULL, &secondDest, getTankAngle((*game).tanks[1]) + 90, NULL, 0);
		if (getHealth((*game).tanks[2])) SDL_RenderCopyEx(renderer, (*game).tankDarkTexture, NULL, &thirdDest, getTankAngle((*game).tanks[2]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankDarkTextureDead, NULL, &thirdDest, getTankAngle((*game).tanks[2]) + 90, NULL, 0);
		if (getHealth((*game).tanks[3])) SDL_RenderCopyEx(renderer, (*game).tankSandTexture, NULL, &fourthDest, getTankAngle((*game).tanks[3]) + 90, NULL, 0);
		else SDL_RenderCopyEx(renderer, (*game).tankSandTextureDead, NULL, &fourthDest, getTankAngle((*game).tanks[3]) + 90, NULL, 0);
		break;
	default:
		break;
	}

	//Skriver på skärmen för att informera om att alla inte är anslutna
	if (*pnrOfConnections<4) {

		(*game).font = TTF_OpenFont("resources/arial.ttf", 45);
		if (!(*game).font) {
			printf("Couldnt load font!!\n");
			getchar();
			getchar();
			SDL_Quit();
			exit(1);
		}

		SDL_Color Color = { 0,0,0 };
		SDL_Surface *tmp;
		tmp = TTF_RenderText_Blended((*game).font, "Waiting for players...", Color);
		(*game).waitFlag = SDL_CreateTextureFromSurface(game->renderer, tmp);

		SDL_Rect textRect = { 470 - tmp->w / 2, 300, tmp->w, tmp->h };

		SDL_RenderCopy(renderer, (*game).waitFlag, NULL, &textRect);

		if (*pnrOfConnections == 4) {
			SDL_FreeSurface(tmp);
			SDL_DestroyTexture((*game).waitFlag);
			for (int m = 0; m < 4; m++)
			{
				resetStructure((*game).tanks[m]);
			}

		}
	}

	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_RenderPresent(renderer);
}

void updateLogic(GameState *game, UDPsocket *sd, IPaddress *srvadd, UDPpacket *p) {
	float xOLD, xNEW, yOLD, yNEW, angleOLD, angleNEW;
	setTankRadian((*game).tanks[0], getTankAngle((*game).tanks[0]) * (M_PI / 180.0));
	setTankDx((*game).tanks[0], cos(getTankRadian((*game).tanks[0])) * getTankVelocity((*game).tanks[0]));
	setTankDy((*game).tanks[0], sin(getTankRadian((*game).tanks[0])) * getTankVelocity((*game).tanks[0]));
	xOLD = getTankPositionX((*game).tanks[0]);
	yOLD = getTankPositionY((*game).tanks[0]);
	angleOLD = getTankAngleOLD((*game).tanks[0]);

	updateTankPositionX((*game).tanks[0], getTankDx((*game).tanks[0]));
	updateTankPositionY((*game).tanks[0], getTankDy((*game).tanks[0]));
	setTankAngleOLD((*game).tanks[0], getTankAngle((*game).tanks[0]));

	xNEW = getTankPositionX((*game).tanks[0]);
	yNEW = getTankPositionY((*game).tanks[0]);
	angleNEW = getTankAngle((*game).tanks[0]);


	for (int i = 0; i < MAX_BULLETS; i++) if ((*game).bullets[i])
	{
		updateBulletPositionX((*game).bullets[i], getBulletDx((*game).bullets[i]));
		updateBulletPositionY((*game).bullets[i], getBulletDy((*game).bullets[i]));
	}

	// printf("OLD: %0.1f\n", getTankAngleOLD((*game).tanks[0]));

	if (xOLD != xNEW || yOLD != yNEW || angleOLD != angleNEW) {

		sprintf((char *)p->data, "%f %f %f\n", xNEW, yNEW, angleNEW);
		p->address.host = (*srvadd).host;    /* Set the destination host */
		p->address.port = (*srvadd).port;    /* And destination port */
		p->len = strlen((char *)p->data) + 1;
		SDLNet_UDP_Send(*sd, -1, p);
	}
}

void handleScore(GameState *game, int* done, SDL_Window* win, UDPsocket *sd, IPaddress *srvadd, UDPpacket *p) {
	int deadCount = 0, nextRound = 0;
	int score[4];
	//Kollar antalet döda spelare
	for (int i = 0; i < 4; i++)
	{
		if (!getHealth((*game).tanks[i])) deadCount++;
	}
	if (deadCount >= 3 && (*game).haveUpdated) {
		(*game).haveUpdated = 0;
		for (int k = 0; k < 4; k++)
		{
			if (getHealth((*game).tanks[k])) {
				setScore((*game).tanks[k], getScore((*game).tanks[k]) + 1);
				break;
			}
		}
		for (int j = 0; j < 4; j++)
		{
			score[j] = getScore((*game).tanks[j]);
		}
		char size[200];
		sprintf(size, "Free-For-All: First to score 3 wins | Score: You:%d, Player2:%d, Player3:%d, Player4:%d", score[0], score[1], score[2], score[3]);
		SDL_SetWindowTitle(win, size);

		if (score[0] == 3 || score[1] == 3 || score[2] == 3 || score[3] == 3) {
			SDL_Delay(3000);
			*done = 1;
		}

		else {
			//Restart for next round
			for (int d = 0; d < MAX_BULLETS; d++)
			{
				destroyBullet((*game).bullets, d);
			}
			for (int m = 0; m < 4; m++)
			{
				resetStructure((*game).tanks[m]);
			}
			SDL_Delay(3000);
			(*game).haveUpdated = 1;

		}
	}
}
