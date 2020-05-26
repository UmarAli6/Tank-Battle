#pragma once
#include <stdio.h>
#include <stdlib.h>
#ifndef bullet_h
#define bullet_h

typedef struct Bullet_type *Bullet;

Bullet createBullet(float x, float y, float dy, float dx, float a, int o);
float getBulletPositionX(Bullet b);
float getBulletPositionY(Bullet b);
void updateBulletPositionX(Bullet b, float x);
void updateBulletPositionY(Bullet b, float y);
void destroyBullet(Bullet bullets[], int i);
float getBulletDy(Bullet b);
float getBulletDx(Bullet b);
float getBulletAngle(Bullet b);
int bulletOwner(Bullet b);

#endif /* Bullet_h */
