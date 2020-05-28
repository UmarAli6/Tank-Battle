#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <SDL.h>
#include "tank.h"
#include "game.h"

#define PUBLIC /* empty */
#define PRIVATE static


struct Tank_type {
	float TANK_POSITION_X;
	float TANK_POSITION_Y;
	float TANK_STARTPOSITION_X;
	float TANK_STARTPOSITION_Y;
	float TANK_VELOCITY;
	float TANK_RADIAN;
	float TANK_ANGLE;
	float TANK_ANGLE_OLD;
	float TANK_STARTANGLE;
	float TANK_DX;
	float TANK_DY;
	int TIMER;
	int ammo[2];
	int health;
	int score;
	int color;
	SDL_Texture* playerTexture;
};

PUBLIC Tank createTank(float x, float y) {
	Tank t = malloc(sizeof(struct Tank_type));
	t->TANK_POSITION_X = x;
	t->TANK_POSITION_Y = y;
	t->TANK_STARTPOSITION_X = x;
	t->TANK_STARTPOSITION_Y = y;
	t->TANK_VELOCITY = 0;
	t->TANK_RADIAN = 0;
	t->TANK_ANGLE = 0;
	t->TANK_ANGLE_OLD = 0;
	t->TANK_DX = 0;
	t->TANK_DY = 0;
	t->TIMER = 0;
	for (int i = 0; i < 3; i++)
	{
		t->ammo[i] = 1;
	}
	t->health = 1;
	t->score = 0;
	return t;
}


PUBLIC float getTankPositionX(Tank t) {
	return t->TANK_POSITION_X;
}

PUBLIC float getTankPositionY(Tank t) {
	return t->TANK_POSITION_Y;
}

PUBLIC void updateTankPositionX(Tank t, float x) {
	t->TANK_POSITION_X += x;
}

PUBLIC void updateTankPositionY(Tank t, float y) {
	t->TANK_POSITION_Y += y;
}

PUBLIC float destroyTank(Tank t) {
	free(t);
	return 1;
}

PUBLIC float setTankY(Tank t, float y)
{
	t->TANK_POSITION_Y = y;
	return t->TANK_POSITION_Y;
}

PUBLIC float setTankX(Tank t, float x)
{
	t->TANK_POSITION_X = x;
	return t->TANK_POSITION_X;
}

PUBLIC float getTankAngle(Tank t) {
	return t->TANK_ANGLE;
}

PUBLIC float getTankAngleOLD(Tank t) {
	return t->TANK_ANGLE_OLD;
}

PUBLIC void updateTankAngle(Tank t, float x) {
	t->TANK_ANGLE = t->TANK_ANGLE + x;
}

PUBLIC void setTankAngle(Tank t, float x)
{
	t->TANK_ANGLE = x;
	t->TANK_STARTANGLE = x;
	t->TANK_ANGLE_OLD = x-1;
}

PUBLIC void setTankAngleOLD(Tank t, float x)
{
	t->TANK_ANGLE_OLD = x;
}

PUBLIC float getTankVelocity(Tank t) {
	return t->TANK_VELOCITY;
}

PUBLIC void setTankVelocity(Tank t, float x) {
	t->TANK_VELOCITY = x;
}

PUBLIC float getTankRadian(Tank t) {
	return t->TANK_RADIAN;
}

PUBLIC void setTankRadian(Tank t, float x) {
	t->TANK_RADIAN = x;
}

PUBLIC float getTankDx(Tank t) {
	return t->TANK_DX;
}

PUBLIC void setTankDx(Tank t, float x) {
	t->TANK_DX = x;
}

PUBLIC float getTankDy(Tank t) {
	return t->TANK_DY;
}

PUBLIC void setTankDy(Tank t, float x) {
	t->TANK_DY = x;
}

PUBLIC void setTimer(Tank t, int x)
{
	t->TIMER = x;
}

PUBLIC int getTimer(Tank t)
{
	return t->TIMER;
}

PUBLIC void setEnemyX(Tank t, float x) {
	t->TANK_POSITION_X = x;
}
PUBLIC void setEnemyY(Tank t, float y) {
	t->TANK_POSITION_Y = y;
}
PUBLIC void setEnemyAngle(Tank t, float x) {
	t->TANK_ANGLE = x;
}
PUBLIC int getAmmo(Tank t, int* a) {
	for (int i = 0; i < 2; i++)
	{
		if (t->ammo[i] == 1) {
			t->ammo[i] = 0; 
			*a = i;
			return 1;
		}
	}
	return 0;
}
PUBLIC void resetAmmo(Tank t, int a) {
	t->ammo[a] = 1;
}
PUBLIC int getHealth(Tank t) {
	return t->health;
}
PUBLIC void setHealth(Tank t, int h) {
	t->health = h;
}
PUBLIC int getScore(Tank t) {
	return t->score;
}
PUBLIC void setScore(Tank t, int s) {
	t->score = s;
}
PUBLIC void resetStructure(Tank t) {
	t->TANK_POSITION_X = t->TANK_STARTPOSITION_X;
	t->TANK_POSITION_Y = t->TANK_STARTPOSITION_Y;
	t->TANK_VELOCITY = 0;
	t->TANK_RADIAN = 0;
	t->TANK_ANGLE = t->TANK_STARTANGLE;
	t->TANK_ANGLE_OLD = t->TANK_STARTANGLE;
	t->TANK_DX = 0;
	t->TANK_DY = 0;
	t->TIMER = 0;
	for (int i = 0; i < 3; i++)
	{
		t->ammo[i] = 1;
	}
	t->health = 1;
}
