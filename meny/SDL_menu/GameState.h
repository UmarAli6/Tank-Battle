//
//  GameState.h
//  SDLxcode
//
//  Created by Yusuf Abdi on 2020-04-18.
//  Copyright © 2020 Yusuf Abdi. All rights reserved.
//

#ifndef GameState_h
#define GameState_h

#include <stdio.h>
#include "SDL2/SDL.h"

typedef struct
{
    float x, y;
    float dx;
    float dy;
    float angle;
    float rad;
    float vel;
    short life;
    char *name;
} Man;

typedef struct
{
    int x,y,w,h;
} LedgeSide;

typedef struct
{
    int x, y, w, h;
} LedgeLong;

typedef struct
{
    int x,y;
} Tank;

typedef struct
{
    //players
    Man man;
    //tanks
    Tank tanks[100];
    //ledges
    LedgeSide ledges[100];
    
    //Images
    SDL_Texture *tank;
    SDL_Texture *brickSide;
    SDL_Texture *brickLong;
    
    //Renderer
    SDL_Renderer *renderer;
} GameState;

void initGame(GameState *game);


#endif /* GameState_h */
