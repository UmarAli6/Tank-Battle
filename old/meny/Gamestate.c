//
//  GameState.c
//  SDLxcode
//
//  Created by Yusuf Abdi on 2020-04-18.
//  Copyright © 2020 Yusuf Abdi. All rights reserved.
//

#include "GameState.h"
#include "SDL2/SDL.h"

void initGame(GameState *game)
{
    // Init tank members
    game->man.x = 320;
    game->man.y = 240;
    game->man.angle = 0.0;
    game->man.vel = 0.0;
    game->man.rad = 0.0;
    
    
    
    //init ledeges
    game->ledges[0].w = 100;
    game->ledges[0].h = 20;
    game->ledges[0].x = 400;
    game->ledges[0].y = 85;
    
    game->ledges[49].w = 20;
    game->ledges[49].h = 90;
    game->ledges[49].x = 250;
    game->ledges[49].y = 0;
    
    game->ledges[50].w = 20;
    game->ledges[50].h = 90;
    game->ledges[50].x = 490;
    game->ledges[50].y = 103;
    
    game->ledges[1].w = 100;
    game->ledges[1].h = 20;
    game->ledges[1].x = 580;
    game->ledges[1].y = 103;
    
    game->ledges[51].w = 20;
    game->ledges[51].h = 90;
    game->ledges[51].x = 490;
    game->ledges[51].y = 280;
    
    game->ledges[2].w = 100;
    game->ledges[2].h = 20;
    game->ledges[2].x = 395;
    game->ledges[2].y = 363;
    
    game->ledges[3].w = 100;
    game->ledges[3].h = 20;
    game->ledges[3].x = 260;
    game->ledges[3].y = 260;
    
    game->ledges[4].w = 100;
    game->ledges[4].h = 20;
    game->ledges[4].x = 260;
    game->ledges[4].y = 170;
    
    game->ledges[52].w = 20;
    game->ledges[52].h = 90;
    game->ledges[52].x = 245;
    game->ledges[52].y = 180;
    
    game->ledges[53].w = 20;
    game->ledges[53].h = 90;
    game->ledges[53].x = 355;
    game->ledges[53].y = 180;
    
    game->ledges[5].w = 100;
    game->ledges[5].h = 20;
    game->ledges[5].x = 120;
    game->ledges[5].y = 363;
    
    game->ledges[6].w = 100;
    game->ledges[6].h = 20;
    game->ledges[6].x = 0;
    game->ledges[6].y = 170;
    
    game->ledges[54].w = 20;
    game->ledges[54].h = 90;
    game->ledges[54].x = 90;
    game->ledges[54].y = 180;
    
    game->ledges[7].w = 100;
    game->ledges[7].h = 20;
    game->ledges[7].x = 160;
    game->ledges[7].y = 90;
}



