#pragma once

#ifndef tank_h
#define tank_h

#include <stdio.h>
#include <stdlib.h>

typedef struct Tank_type *Tank;

Tank createTank(int x, int y);
int getTankPositionX(Tank t);
int getTankPositionY(Tank t);
void updateTankPositionX(Tank t, int x);
void updateTankPositionY(Tank t, int y);
int destroyTank(Tank t);

#endif /* Tank_h */
