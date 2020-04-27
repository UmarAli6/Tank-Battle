#include "tank.h"

#define PUBLIC /* empty */
#define PRIVATE static

struct Tank_type {
	float TANK_POSITION_X;
	float TANK_POSITION_Y;
	float TANK_VELOCITY;
	float TANK_RADIAN;
	float TANK_ANGLE;
	float TANK_DX;
	float TANK_DY;
	int TIMER;
};

PUBLIC Tank createTank(float x, float y) {
	Tank t = malloc(sizeof(struct Tank_type));
	t->TANK_POSITION_X = x;
	t->TANK_POSITION_Y = y;
	t->TANK_VELOCITY = 0;
	t->TANK_RADIAN = 0;
	t->TANK_ANGLE = 0;
	t->TANK_DX = 0;
	t->TANK_DY = 0;
	t->TIMER = 0;

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
}

PUBLIC float setTankX(Tank t, float x)
{
	t->TANK_POSITION_X = x;
}

PUBLIC float getTankAngle(Tank t) {
	return t->TANK_ANGLE;
}

PUBLIC void updateTankAngle(Tank t, float x) {
	t->TANK_ANGLE = t->TANK_ANGLE + x;
}

PUBLIC void setTankAngle(Tank t, float x)
{
	t->TANK_ANGLE = x;
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
