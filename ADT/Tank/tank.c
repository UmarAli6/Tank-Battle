#include "tank.h"

#define PUBLIC /* empty */
#define PRIVATE static

struct Tank_type {
	int TANK_POSITION_X;
	int TANK_POSITION_Y;
	int TANK_FRAME;
	int speed;
};

PUBLIC Tank createTank(int x, int y) {
	Tank t = malloc(sizeof(struct Tank_type));
	t->TANK_POSITION_X = x;
	t->TANK_POSITION_Y = y;
	t->TANK_FRAME = 0;
	t->speed = 1;
	return t;
}

PUBLIC int getTankPositionX(Tank t) {
	return t->TANK_POSITION_X;
}

PUBLIC int getTankPositionY(Tank t) {
	return t->TANK_POSITION_Y;
}

PUBLIC void updateTankPositionX(Tank t, int x) {
	t->TANK_POSITION_X += x;
}

PUBLIC void updateTankPositionY(Tank t, int y) {
	t->TANK_POSITION_Y += y;
}

PUBLIC int destroyTank(Tank t) {
	free(t);
	return 1;
}
