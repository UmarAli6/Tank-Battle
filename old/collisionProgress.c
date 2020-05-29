#define MAX_BULLETS 1000
#define MAX_WALLS 50
#define WIDTH	960
#define HEIGHT	720	

typedef struct
{
	int x, y, w, h;
} Wall;

typedef struct
{
	//Bullet
	Bullet bullets[MAX_BULLETS];

	//Tanks
	Tank t1;

	//Walls
	Wall walls[MAX_WALLS];

	//Images
	SDL_Texture *bild;
	SDL_Texture *brickSide;
	SDL_Texture *brickLong;
	SDL_Texture *bulletGreen;

	//Renderer
	SDL_Renderer *renderer;
} GameState;





// Collision progress

void collisionDetect(GameState *game)
{
	//Bullets within window
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (game->bullets[i] != NULL)
		{
			{
				if (getBulletPositionX(game->bullets[i]) < -20 || getBulletPositionX(game->bullets[i]) > 700)
				{
					destroyBullet(game->bullets[i], i);
					game->bullets[i] = NULL;
				}
				else if (getBulletPositionY(game->bullets[i]) < -20 || getBulletPositionY(game->bullets[i]) > 700)
				{
					destroyBullet(game->bullets[i], i);
					game->bullets[i] = NULL;
				}
			}
		}
	}

	//The sides of tank and wall
	int tankLeft, wallLeft;
	int tankRight, wallRight;
	int tankTop, wallTop;
	int tankBottom, wallBottom;

	
	//Check for collision with any walls and frame
	for (int i = 0; i < MAX_WALLS; i++)
	{
		//Calculate the sides of the tank
		tankLeft = getTankPositionX(game->t1);
		tankRight = getTankPositionX(game->t1) + 48;
		tankTop = getTankPositionY(game->t1);
		tankBottom = getTankPositionY(game->t1) + 48;

		//Calculate the sides of the wall
		wallLeft = game->walls[i].x;
		wallRight = game->walls[i].x + game->walls[i].w;
		wallTop = game->walls[i].y;
		wallBottom = game->walls[i].y + game->walls[i].h;
		

		//If all side of the tank is outside of the wall, if not -> collision

		if (tankBottom >= wallTop && tankTop <= wallBottom && tankRight >= wallLeft && tankLeft <= wallRight)
		{
			if (game->walls[i].w > 5) {
				
				setTankY(game->t1, (getTankPositionY(game->t1)) - getTankDy(game->t1));

			}




			//setTankY(game->t1, (getTankPositionY(game->t1)) - getTankDy(game->t1));
			//setTankX(game->t1, (getTankPositionX(game->t1)) - getTankDx(game->t1));
			
		}
	}
}
