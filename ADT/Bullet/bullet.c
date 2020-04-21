#include "bullet.h"

#define PUBLIC /* empty */
#define PRIVATE static

struct Bullet_type {
	int BULLET_POSITION_X;
	int BULLET_POSITION_Y;
	int speed;
	int bouncesLeft;
};

PUBLIC Bullet createBullet(int x, int y) {
	Bullet b = malloc(sizeof(struct Bullet_type));
	b->BULLET_POSITION_X = x;
	b->BULLET_POSITION_Y = y;
	b->speed = 1;
	b->bouncesLeft = 4;
	return b;
}

PUBLIC int getBulletPositionX(Bullet b) {
	return b->BULLET_POSITION_X;
}

PUBLIC int getBulletPositionY(Bullet b) {
	return b->BULLET_POSITION_Y;
}

PUBLIC int destroyBullet(Bullet b) {
	free(b);
	return 1;
}
