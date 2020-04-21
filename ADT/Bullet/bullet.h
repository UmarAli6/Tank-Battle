#pragma once

#ifndef bullet_h
#define bullet_h

#include <stdio.h>
#include <stdlib.h>

typedef struct Bullet_type *Bullet;

Bullet createBullet(int x, int y);
int getBulletPositionX(Bullet b);
int getBulletPositionY(Bullet b);
int destroyBullet(Bullet b);

#endif /* Bullet_h */
