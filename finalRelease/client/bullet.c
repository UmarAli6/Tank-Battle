#include "bullet.h"

#define PUBLIC /* empty */
#define PRIVATE static

struct Bullet_type {
	float BULLET_POSITION_X;
	float BULLET_POSITION_Y;
	float BULLET_DY;
	float BULLET_DX;
	float BULLET_ANGLE;
	int OWNER;
};

PUBLIC Bullet createBullet(float x, float y, float dy, float dx, float a, int o) {
	Bullet b = malloc(sizeof(struct Bullet_type));
	b->BULLET_POSITION_X = x;
	b->BULLET_POSITION_Y = y;
	b->BULLET_DY = dy * -8;
	b->BULLET_DX = dx * -8;
	b->BULLET_ANGLE = a;
	b->OWNER = o; //Called entity
	return b;
}

PUBLIC void updateBulletPositionX(Bullet b, float x)
{
	b->BULLET_POSITION_X += x;
}

PUBLIC void updateBulletPositionY(Bullet b, float y)
{
	b->BULLET_POSITION_Y += y;
}

PUBLIC float getBulletPositionX(Bullet b) {
	return b->BULLET_POSITION_X;
}

PUBLIC float getBulletPositionY(Bullet b) {
	return b->BULLET_POSITION_Y;
}

PUBLIC void destroyBullet(Bullet bullets[], int i) {
	free(bullets[i]);
	bullets[i] = NULL;
}

PUBLIC float getBulletDx(Bullet b)
{
	return b->BULLET_DX;
}

PUBLIC float getBulletDy(Bullet b)
{
	return b->BULLET_DY;
}

PUBLIC float getBulletAngle(Bullet b)
{
	return b->BULLET_ANGLE;
}

PUBLIC int bulletOwner(Bullet b) {
	return b->OWNER;
}