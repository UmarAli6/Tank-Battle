#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include <string.h>
#include <SDL_ttf.h>

#define WIDTH 960
#define HEIGHT 720
#define MAX_LEDGES 24

void loadGame(GameState* game, int *pnrOfConnections, UDPsocket *sd, IPaddress *srvadd, UDPpacket *p, UDPpacket *p2, int *pWhich, SDL_Window* win) {
	SDL_Surface *surface = NULL;
	//Load images and create rendering textures from them
	surface = IMG_Load("resources/tank_sand.png");
	(*game).tankSandTexture = SDL_CreateTextureFromSurface((*game).renderer, surface);
	surface = IMG_Load("resources/tank_sand_dead.png");
	(*game).tankSandTextureDead = SDL_CreateTextureFromSurface((*game).renderer, surface);

	surface = IMG_Load("resources/tank_green.png");
	(*game).tankGreenTexture = SDL_CreateTextureFromSurface((*game).renderer, surface);
	surface = IMG_Load("resources/tank_green_dead.png");
	(*game).tankGreenTextureDead = SDL_CreateTextureFromSurface((*game).renderer, surface);

	surface = IMG_Load("resources/tank_dark.png");
	(*game).tankDarkTexture = SDL_CreateTextureFromSurface((*game).renderer, surface);
	surface = IMG_Load("resources/tank_dark_dead.png");
	(*game).tankDarkTextureDead = SDL_CreateTextureFromSurface((*game).renderer, surface);

	surface = IMG_Load("resources/tank_blue.png");
	(*game).tankBlueTexture = SDL_CreateTextureFromSurface((*game).renderer, surface);
	surface = IMG_Load("resources/tank_blue_dead.png");
	(*game).tankBlueTextureDead = SDL_CreateTextureFromSurface((*game).renderer, surface);

	surface = IMG_Load("resources/bullet.png");
	(*game).bullet = SDL_CreateTextureFromSurface((*game).renderer, surface);
	SDL_FreeSurface(surface);

	// Decide tank setup depending on my own player index
	if (*pnrOfConnections == 1) {
		(*game).tanks[0] = createTank(10, 10);
		setTankAngle((*game).tanks[0], 270);
		(*game).tanks[1] = createTank(10, 660);
		setTankAngle((*game).tanks[1], 90);
		(*game).tanks[2] = createTank(880, 10);
		setTankAngle((*game).tanks[2], 270);
		(*game).tanks[3] = createTank(880, 660);
		setTankAngle((*game).tanks[3], 90);
		*pWhich = 1;
		sprintf((char *)p->data, "%f %f %f\n", 10.0f, 10.0f, -90.0f); //We are sending this data just to update nrOfConnections faster to remove waitFlag and call resetStructure
		p->address.host = (*srvadd).host;    /* Set the destination host */
		p->address.port = (*srvadd).port;    /* And destination port */
		p->len = strlen((char *)p->data) + 1;
		SDLNet_UDP_Send(*sd, -1, p);
	}
	if (*pnrOfConnections == 2) {

		(*game).tanks[1] = createTank(10, 10);
		setTankAngle((*game).tanks[1], 270);
		(*game).tanks[0] = createTank(880, 10);
		setTankAngle((*game).tanks[0], 270);
		(*game).tanks[2] = createTank(880, 660);
		setTankAngle((*game).tanks[2], 90);
		(*game).tanks[3] = createTank(10, 660);
		setTankAngle((*game).tanks[3], 90);
		*pWhich = 2;
		sprintf((char *)p->data, "%f %f %f\n", 880.0f, 10.0f, -90.0f);
		p->address.host = (*srvadd).host;    /* Set the destination host */
		p->address.port = (*srvadd).port;    /* And destination port */
		p->len = strlen((char *)p->data) + 1;
		SDLNet_UDP_Send(*sd, -1, p);
	}
	if (*pnrOfConnections == 3) {

		(*game).tanks[1] = createTank(880, 660);
		setTankAngle((*game).tanks[1], 90);
		(*game).tanks[2] = createTank(10, 10);
		setTankAngle((*game).tanks[2], 270);
		(*game).tanks[0] = createTank(10, 660);
		setTankAngle((*game).tanks[0], 90);
		(*game).tanks[3] = createTank(880, 10);
		setTankAngle((*game).tanks[3], 270);
		*pWhich = 3;
		sprintf((char *)p->data, "%f %f %f\n", 10.0f, 660.0f, 90.0f);
		p->address.host = (*srvadd).host;    /* Set the destination host */
		p->address.port = (*srvadd).port;    /* And destination port */
		p->len = strlen((char *)p->data) + 1;
		SDLNet_UDP_Send(*sd, -1, p);
	}
	if (*pnrOfConnections == 4) {

		(*game).tanks[1] = createTank(880, 10);
		setTankAngle((*game).tanks[1], 270);
		(*game).tanks[2] = createTank(10, 660);
		setTankAngle((*game).tanks[2], 90);
		(*game).tanks[3] = createTank(10, 10);
		setTankAngle((*game).tanks[3], 270);
		(*game).tanks[0] = createTank(880, 660);
		setTankAngle((*game).tanks[0], 90);
		*pWhich = 4;
		sprintf((char *)p->data, "%f %f %f\n", 880.0f, 660.0f, 90.0f);
		p->address.host = (*srvadd).host;    /* Set the destination host */
		p->address.port = (*srvadd).port;    /* And destination port */
		p->len = strlen((char *)p->data) + 1;
		SDLNet_UDP_Send(*sd, -1, p);
	}

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		(*game).bullets[i] = NULL;
	}

	//init ledeges (Map)
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
	(*game).changedGameState = 0;

	char size[100];
	sprintf(size, "Free-For-All: First to score 3 wins | Score: You:0, Player2:0, Player3:0, Player4:0");
	SDL_SetWindowTitle(win, size); //Update game windows title with score
}

void collisionDetect(GameState* game) {

	//The sides of tank, wall and bullet
	int tankLeft, wallLeft, bulletLeft;
	int tankRight, wallRight, bulletRight;
	int tankTop, wallTop, bulletTop;
	int tankBottom, wallBottom, bulletBottom;


	//Calculate the sides of the tank
	tankLeft = getTankPositionX((*game).tanks[0]);
	tankRight = getTankPositionX((*game).tanks[0]) + 48;
	tankTop = getTankPositionY((*game).tanks[0]);
	tankBottom = getTankPositionY((*game).tanks[0]) + 48;

	//Check for collision with any ledges and frame
	for (int i = 0; i < MAX_LEDGES; i++)
	{

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

	//Check for collision between tanks
	for (int i = 0; i < 4; i++)
	{
		//Calculate the sides of the tank
		tankLeft = getTankPositionX((*game).tanks[i]);
		tankRight = getTankPositionX((*game).tanks[i]) + 48;
		tankTop = getTankPositionY((*game).tanks[i]);
		tankBottom = getTankPositionY((*game).tanks[i]) + 48;

		for (int j = 0; j < 4; j++)
		{
			//Calculate the sides of the tank
			int nxtTankLeft = getTankPositionX((*game).tanks[j]);
			int nxtTankRight = getTankPositionX((*game).tanks[j]) + 48;
			int nxtTankTop = getTankPositionY((*game).tanks[j]);
			int nxtTankBottom = getTankPositionY((*game).tanks[j]) + 48;

			if ((tankBottom >= nxtTankTop && tankTop <= nxtTankBottom && tankRight >= nxtTankLeft && tankLeft <= nxtTankRight) && (j != i))
			{
				setTankY((*game).tanks[0], (getTankPositionY((*game).tanks[0])) - getTankDy((*game).tanks[0]));
				setTankX((*game).tanks[0], (getTankPositionX((*game).tanks[0])) - getTankDx((*game).tanks[0]));
			}
		}

	}

	
	int bDestroyed;
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bDestroyed = 0;
		//Removes bullets when colliding with tanks and set hp to 0
		if ((*game).bullets[i] != NULL) {

			//Calculate the sides of the bullet
			bulletLeft = getBulletPositionX((*game).bullets[i]);
			bulletRight = getBulletPositionX((*game).bullets[i]) + 10;
			bulletTop = getBulletPositionY((*game).bullets[i]);
			bulletBottom = getBulletPositionY((*game).bullets[i]) + 10;

			for (int j = 0; j < 4 && (*game).bullets[i] != NULL; j++)
			{
				//Calculate the sides of the tank
				tankLeft = getTankPositionX((*game).tanks[j]);
				tankRight = getTankPositionX((*game).tanks[j]) + 48;
				tankTop = getTankPositionY((*game).tanks[j]);
				tankBottom = getTankPositionY((*game).tanks[j]) + 48;

				if (bulletBottom >= tankTop && bulletTop <= tankBottom && bulletRight >= tankLeft && bulletLeft <= tankRight) {
					if (bulletOwner((*game).bullets[i]) != j) {
						setHealth((*game).tanks[j], 0);
						destroyBullet(game->bullets, i);
						bDestroyed = 1;
					}
				}
			}

			//Removes bullets when colliding with walls
			if (!bDestroyed && (*game).bullets[i] != NULL) {

				for (int j = 0; j < MAX_LEDGES && bDestroyed == 0; j++) {

					//Calculate the sides of the wall
					wallLeft = game->ledges[j].x;
					wallRight = game->ledges[j].x + game->ledges[j].w;
					wallTop = game->ledges[j].y;
					wallBottom = game->ledges[j].y + game->ledges[j].h;

					//If all side of the bullet is outside of the wall, if not -> collision
					if (bulletBottom >= wallTop && bulletTop <= wallBottom && bulletRight >= wallLeft && bulletLeft <= wallRight)
					{
						destroyBullet(game->bullets, i);
						bDestroyed = 1;
					}
				}
			}

			//Reload so the player can shoot again, max 2 bullets available at the same time
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
	if (!(*game).changedGameState && *pNrOfConnections == 4) {
		(*game).changedGameState = 1;
		for (int m = 0; m < 4; m++)
		{
			resetStructure((*game).tanks[m]);	
		}
		return done;
	}
	while (SDL_PollEvent(&event))
	{	
		//Checking if we should close game
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
			done = 1;
			break;
		}
	}
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	//Handle events for key down
	if (state[SDL_SCANCODE_LEFT] && getHealth((*game).tanks[0])) //Also check if player is alive 
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
	if (state[SDL_SCANCODE_SPACE] && getHealth((*game).tanks[0]) && *pNrOfConnections == 4) //Check if alive and everyone is connected, if so then bullet can be fired
	{
		int returnedNumber;
		currentTime = SDL_GetTicks();
		if (currentTime > getTimer((*game).tanks[0]) + 500) //Prevent bullet spam
		{
			if (getAmmo((*game).tanks[0], &returnedNumber)) {
				//Send data sequence that bulled was fired, -5-5-5
				float xNEW = -5.0f, yNEW = -5.0f, angleNEW = -5.0f;
				sprintf((char *)p->data, "%f %f %f\n", xNEW, yNEW, angleNEW);
				p->address.host = (*srvadd).host;    /* Set the destination host */
				p->address.port = (*srvadd).port;    /* And destination port */
				p->len = strlen((char *)p->data) + 1;
				SDLNet_UDP_Send(*sd, -1, p);
				//Create bullet and add to bullet list to remember how many bullets can be fired next time space is down
				(*game).bullets[returnedNumber] = createBullet(getTankPositionX((*game).tanks[0]), getTankPositionY((*game).tanks[0]), sin(getTankRadian((*game).tanks[0])), cos(getTankRadian((*game).tanks[0])), getTankAngle((*game).tanks[0]), 0);
				setTimer((*game).tanks[0], currentTime);
			}
		}
	}
	if (state[SDL_SCANCODE_UP] && getHealth((*game).tanks[0])) //Set speed of tank if alive
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
	int client, returnedNumber;
	//UDP packet recived
	if (SDLNet_UDP_Recv(*sd, p2)) {
		sscanf((char *)p2->data, "%f %f %d %f %d\n", &xCORD, &yCORD, &client, &dAngle, pnrOfConnections);
		if (client == 1) { //Keep track of what client sent the data
			if (xCORD == -6 && yCORD == -6 && dAngle == -6) { //Data sequence that client fired a bulled, -6-6-6
				if (getAmmo((*game).tanks[1], &returnedNumber)) {
					setTankRadian((*game).tanks[1], getTankAngle((*game).tanks[1]) * (M_PI / 180.0));
					(*game).bullets[client * 2 + returnedNumber] = createBullet(getTankPositionX((*game).tanks[1]), getTankPositionY((*game).tanks[1]), sin(getTankRadian((*game).tanks[1])), cos(getTankRadian((*game).tanks[1])), getTankAngle((*game).tanks[1]), client);
					setTimer((*game).tanks[1], SDL_GetTicks());
				}
			}
			else { //Normal data (Movement of tank)
				setEnemyX((*game).tanks[1], xCORD);
				setEnemyY((*game).tanks[1], yCORD);
				setEnemyAngle((*game).tanks[1], dAngle);
			}
		}
		if (client == 2) {
			if (xCORD == -6 && yCORD == -6 && dAngle == -6) {
				if (getAmmo((*game).tanks[2], &returnedNumber)) {
					setTankRadian((*game).tanks[2], getTankAngle((*game).tanks[2]) * (M_PI / 180.0));
					(*game).bullets[client * 2 + returnedNumber] = createBullet(getTankPositionX((*game).tanks[2]), getTankPositionY((*game).tanks[2]), sin(getTankRadian((*game).tanks[2])), cos(getTankRadian((*game).tanks[2])), getTankAngle((*game).tanks[2]), client);
					setTimer((*game).tanks[2], SDL_GetTicks());
				}
			}
			else {
				setEnemyX((*game).tanks[2], xCORD);
				setEnemyY((*game).tanks[2], yCORD);
				setEnemyAngle((*game).tanks[2], dAngle);
			}
		}
		if (client == 3) {
			if (xCORD == -6 && yCORD == -6 && dAngle == -6) {
				if (getAmmo((*game).tanks[3], &returnedNumber)) {
					setTankRadian((*game).tanks[3], getTankAngle((*game).tanks[3]) * (M_PI / 180.0));
					(*game).bullets[client * 2 + returnedNumber] = createBullet(getTankPositionX((*game).tanks[3]), getTankPositionY((*game).tanks[3]), sin(getTankRadian((*game).tanks[3])), cos(getTankRadian((*game).tanks[3])), getTankAngle((*game).tanks[3]), client);
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

	//Clear the screen (to beige)
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 192, 192, 192, 0);

	//Clear the ledges (to Black)
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	//Inform players that lobby isnt full
	if (*pnrOfConnections != 4) {
		SDL_Color Color = { 0,0,0 };
		SDL_Surface *tmp;
		tmp = TTF_RenderText_Blended((*game).font, "Waiting for players...", Color);
		if (tmp != NULL) {
			(*game).waitFlag = SDL_CreateTextureFromSurface(game->renderer, tmp);
			SDL_Rect textRect = { 470 - tmp->w / 2, 300, tmp->w, tmp->h };
			SDL_RenderCopy(renderer, (*game).waitFlag, NULL, &textRect);
			SDL_FreeSurface(tmp);
		}
	}

	//Draw ledges on map
	for (int i = 0; i <MAX_LEDGES; i++)
	{
		SDL_Rect ledgeRect = { game->ledges[i].x,game->ledges[i].y,game->ledges[i].w,game->ledges[i].h };
		SDL_RenderFillRect(renderer, &ledgeRect);
	}

	//Draw bullets positions
	for (int i = 0; i < MAX_BULLETS; i++) if ((*game).bullets[i])
	{
		SDL_Rect bullrect = { getBulletPositionX((*game).bullets[i]) + 20, getBulletPositionY((*game).bullets[i]) + 24, 10, 10 };
		SDL_RenderCopyEx(renderer, (*game).bullet, NULL, &bullrect, getBulletAngle((*game).bullets[i]) + 90, NULL, SDL_FLIP_VERTICAL);
	}
	//Get tank positions
	SDL_Rect rect = { getTankPositionX((*game).tanks[0]), getTankPositionY((*game).tanks[0]),48,48 };
	SDL_Rect secondDest = { getTankPositionX((*game).tanks[1]), getTankPositionY((*game).tanks[1]) ,48,48 };
	SDL_Rect thirdDest = { getTankPositionX((*game).tanks[2]), getTankPositionY((*game).tanks[2]) ,48,48 };
	SDL_Rect fourthDest = { getTankPositionX((*game).tanks[3]), getTankPositionY((*game).tanks[3]) ,48,48 };

	switch (*pWhich) //Check playerIndex to determine right image for the tanks, color and if dead, then draw
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

	//Update bullet position
	for (int i = 0; i < MAX_BULLETS; i++) if ((*game).bullets[i])
	{
		updateBulletPositionX((*game).bullets[i], getBulletDx((*game).bullets[i]));
		updateBulletPositionY((*game).bullets[i], getBulletDy((*game).bullets[i]));
	}
	//If tank has moved send data to server
	if (xOLD != xNEW || yOLD != yNEW || angleOLD != angleNEW) {
		sprintf((char *)p->data, "%f %f %f\n", xNEW, yNEW, angleNEW);
		p->address.host = (*srvadd).host;    /* Set the destination host */
		p->address.port = (*srvadd).port;    /* And destination port */
		p->len = strlen((char *)p->data) + 1;
		SDLNet_UDP_Send(*sd, -1, p);
	}
}

void handleScore(GameState *game, int* gameOver, SDL_Window* win) {
	int deadCount = 0, nextRound = 0;
	int score[4];
	//Get player dead count
	for (int i = 0; i < 4; i++)
	{
		if (!getHealth((*game).tanks[i])) deadCount++;
	}
	if (deadCount >= 3 && (*game).haveUpdated) {
		(*game).haveUpdated = 0;
		for (int k = 0; k < 4; k++)
		{
			//Get last player alive and add score
			if (getHealth((*game).tanks[k])) {
				setScore((*game).tanks[k], getScore((*game).tanks[k]) + 1);
				break;
			}
		}
		for (int j = 0; j < 4; j++)
		{
			//Get all tanks score
			score[j] = getScore((*game).tanks[j]);
		}
		char size[100];
		sprintf(size, "Free-For-All: First to score 3 wins | Score: You:%d, Player2:%d, Player3:%d, Player4:%d", score[0], score[1], score[2], score[3]);
		SDL_SetWindowTitle(win, size); //Update game windows title with score
		if (score[0] == 3 || score[1] == 3 || score[2] == 3 || score[3] == 3) *gameOver = 1; //If player reaches 3 points the game is over
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

void resetGame(GameState *game, UDPsocket *sd, IPaddress *srvadd, UDPpacket *p, int *pnrOfConnections, int* gameOver, SDL_Window* win) {
	SDL_DestroyTexture((*game).tankSandTexture);
	SDL_DestroyTexture((*game).tankDarkTexture);
	SDL_DestroyTexture((*game).tankGreenTexture);
	SDL_DestroyTexture((*game).tankBlueTexture);
	SDL_DestroyTexture((*game).tankSandTextureDead);
	SDL_DestroyTexture((*game).tankDarkTextureDead);
	SDL_DestroyTexture((*game).tankGreenTextureDead);
	SDL_DestroyTexture((*game).tankBlueTextureDead);
	SDL_DestroyTexture((*game).bullet);

	for (int i = 0; i < 4; i++)
	{
		destroyTank((*game).tanks[i]);
	}
	for (int j = 0; j < MAX_BULLETS; j++)
	{
		if ((*game).bullets[j] != NULL) {
			destroyBullet((*game).bullets, j);
		}
	}
	*pnrOfConnections = 0;
	sprintf((char *)p->data, "%f %f %f\n", -7.0f, -7.0f, -7.0f); //Data sequence to inform server we are done playing
	p->address.host = (*srvadd).host;    /* Set the destination host */
	p->address.port = (*srvadd).port;    /* And destination port */
	p->len = strlen((char *)p->data) + 1;
	SDLNet_UDP_Send(*sd, -1, p);
	printf("Game over\n");
	(*gameOver) = 0;
	char size[50];
	sprintf(size, "Tank Battle - connect using the terminal");
	SDL_SetWindowTitle(win, size); //Update game windows title with main menu title
	
}