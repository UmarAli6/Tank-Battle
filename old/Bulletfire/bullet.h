#pragma once

#ifndef bullet_h
#define bullet_h

#include <stdio.h>
#include <stdlib.h>

typedef struct Bullet_type *Bullet;

Bullet createBullet(float x, float y, float dy, float dx, float a);
float getBulletPositionX(Bullet b);
float getBulletPositionY(Bullet b);
void updateBulletPositionX(Bullet b, float x);
void updateBulletPositionY(Bullet b, float y);
void destroyBullet(Bullet bullets[], int i);
float getBulletDy(Bullet b);
float getBulletDx(Bullet b);
float getBulletAngle(Bullet b);

#endif /* Bullet_h */
