#pragma once

#ifndef game_h
#define game_h

#define MAX_BULLETS 8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include "tank.h"
#include "bullet.h"

struct Vec2 {
    float x, y;
};

struct Player {
    Uint32 health;
    Uint32 image;
    Uint32 ammo;
    char name[16];
    struct Vec2 vec2;
    SDL_Rect playerPos;
};

struct Gamestate {
    struct Player players[4];
    Uint32 playerNumber;
    Uint32 score[4];
};

typedef struct
{
    int x, y, w, h;
} LedgeSide;

typedef struct
{
    int x, y, w, h;
} LedgeLong;

typedef struct
{
    //Bullet
    Bullet bullets[MAX_BULLETS];
    //Tanks
    Tank tanks[4];
    //ledges
    LedgeSide ledges[100];
    
    //Images
    SDL_Texture *tankSandTexture;
    SDL_Texture *tankGreenTexture;
    SDL_Texture *tankDarkTexture;
    SDL_Texture *tankBlueTexture;
    
    SDL_Texture *tankSandTextureDead;
    SDL_Texture *tankGreenTextureDead;
    SDL_Texture *tankDarkTextureDead;
    SDL_Texture *tankBlueTextureDead;
    
    SDL_Texture *bullet;
    
    SDL_Texture *waitFlag;
    SDL_Texture *label[3];
    
    //Font
    TTF_Font *font;
    
    //Renderer
    SDL_Renderer *renderer;
    
    int haveUpdated;
} GameState;

void loadGame(GameState* game, int *pnrOfConnections, UDPsocket *sd, IPaddress *srvadd, UDPpacket *p, UDPpacket *p2, int *pWhich);
void collisionDetect(GameState *game);
int processEvents(SDL_Window *window, GameState *game, UDPsocket *sd, IPaddress *srvadd, UDPpacket *p, int *pNrOfConnections);
void doRender(SDL_Renderer *renderer, GameState *game, int *pnrOfConnections, UDPsocket *sd, UDPpacket *p2, int *pWhich);
void updateLogic(GameState *game, UDPsocket *sd, IPaddress *srvadd, UDPpacket *p);
void handleScore(GameState *game, int* gameOver, SDL_Window* win, UDPsocket *sd, IPaddress *srvadd, UDPpacket *p);

#endif /* game.h */
