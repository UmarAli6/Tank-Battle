//
//  MainMenu.c
//  SDLxcode
//
//  Created by Yusuf Abdi on 2020-04-18.
//  Copyright © 2020 Yusuf Abdi. All rights reserved.
//

#include "MainMenu.h"


int previewChoice(int Mx, int My, SDL_Rect Rect, SDL_Texture *Tex)
{
    int test=0;
    
    if (Mx >= Rect.x && Mx <= Rect.x + Rect.w && My >= Rect.y && My <= Rect.y + Rect.h){
        
        SDL_SetTextureColorMod(Tex, 250, 0, 0 );
        test=1;
    }
    else
    {
        SDL_SetTextureColorMod(Tex, 250, 250, 250);
    }
    return test;
}
