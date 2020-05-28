#pragma once

#ifndef tank_h
#define tank_h

#include <stdio.h>
#include <stdlib.h>

typedef struct Tank_type *Tank;

Tank createTank(float x, float y);
float destroyTank(Tank t);

float getTankPositionX(Tank t);
float getTankPositionY(Tank t);

void updateTankPositionX(Tank t, float x);
void updateTankPositionY(Tank t, float y);

float setTankX(Tank t, float x);
float setTankY(Tank t, float y);

void setTankRadian(Tank t, float x);
float getTankRadian(Tank t);

void setTankVelocity(Tank t, float x);
float getTankVelocity(Tank t);

void updateTankAngle(Tank t, float x);
float getTankAngle(Tank t);
void setTankAngle(Tank t, float x);

void setTankDy(Tank t, float x);
float getTankDy(Tank t);

void setTankDx(Tank t, float x);
float getTankDx(Tank t);

int getTimer(Tank t);
void setTimer(Tank t, int x);

void setEnemyX(Tank t, float x);
void setEnemyY(Tank t, float y);

void setEnemyAngle(Tank t, float x);
void setTankAngleOLD(Tank t, float x);
float getTankAngleOLD(Tank t);

int getAmmo(Tank t, int* a);
void resetAmmo(Tank t, int a);

int getHealth(Tank t);
void setHealth(Tank t, int h);

int getScore(Tank t);
void setScore(Tank t, int s);

void resetStructure(Tank t);

#endif /* tank_h */
