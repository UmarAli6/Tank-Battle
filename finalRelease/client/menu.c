#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h">
#include "network.h"

int showMenu(GameState *game)
{
    (*game).font = TTF_OpenFont("resources/arial.ttf", 30);
    if (!(*game).font) {
        printf("Couldnt load font!!\n");
        getchar();
        getchar();
        SDL_Quit();
        exit(1);
    }
    
    SDL_Color textColor[2] = { { 255,255,255 },{ 255,0,0 } };
    const int nrOptions = 3;
    int x, y;
    SDL_Surface *tmp[3];
    tmp[0] = TTF_RenderText_Blended((*game).font, "Start game", textColor[0]);
    (*game).label[0] = SDL_CreateTextureFromSurface((*game).renderer, tmp[0]);
    
    tmp[1] = TTF_RenderText_Blended((*game).font, "How to play", textColor[0]);
    (*game).label[1] = SDL_CreateTextureFromSurface((*game).renderer, tmp[1]);
    
    tmp[2] = TTF_RenderText_Blended((*game).font, "Exit", textColor[0]);
    (*game).label[2] = SDL_CreateTextureFromSurface((*game).renderer, tmp[2]);
    
    
    SDL_Rect textRect[3];
    SDL_Rect howTo;
    textRect[0].x = 460 - tmp[0]->w / 2;
    textRect[0].y = 275;
    textRect[0].w = tmp[0]->w;
    textRect[0].h = tmp[0]->h;
    
    textRect[1].x = 460 - tmp[1]->w / 2;
    textRect[1].y = 295 + tmp[0]->h;
    textRect[1].w = tmp[1]->w;
    textRect[1].h = tmp[1]->h;
    
    textRect[2].x = 460 - tmp[2]->w / 2;
    textRect[2].y = 315 + tmp[0]->h + tmp[1]->h;
    textRect[2].w = tmp[2]->w;
    textRect[2].h = tmp[2]->h;
    
    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    for (int i = 0; i< nrOptions; i++)
                    {
                        SDL_FreeSurface(tmp[i]);
                        SDL_DestroyTexture((*game).label[i]);
                    }
                    return 1;
                    break;
                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    for (int i = 0; i < nrOptions; i++) {
                        if (x >= textRect[i].x && x <= textRect[i].x + textRect[i].w && y >= textRect[i].y && y <= textRect[i].y + textRect[i].h)
                        {
                            SDL_SetTextureColorMod((*game).label[i], 250, 0, 0);
                        }
                        else
                        {
                            SDL_SetTextureColorMod((*game).label[i], 255, 255, 255);
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x;
                    y = event.button.y;
                    for (int i = 0; i < nrOptions; i++) {
                        if (x >= textRect[i].x && x <= textRect[i].x + textRect[i].w && y >= textRect[i].y && y <= textRect[i].y + textRect[i].h)
                        {
                            if (x >= textRect[1].x && x <= textRect[1].x + textRect[1].w && y >= textRect[1].y && y <= textRect[1].y + textRect[1].h) {
                                
                                SDL_Surface *test;
                                test = IMG_Load("resources/tutorial.png");
                                SDL_Texture *Tex = SDL_CreateTextureFromSurface((*game).renderer, test);
                                int quit = 0;
                                while (!quit) {
                                    while (SDL_PollEvent(&event)) {
                                        switch (event.type) {
                                            case SDL_QUIT:
                                                for (int i = 0; i< nrOptions; i++)
                                                {
                                                    SDL_FreeSurface(tmp[i]);
                                                    SDL_DestroyTexture((*game).label[i]);
                                                }
                                                return 1;
                                                
                                            case SDL_MOUSEBUTTONDOWN:
                                                SDL_FreeSurface(test);
                                                SDL_DestroyTexture(Tex);
                                                quit = 1;
                                                break;
                                                
                                            default:
                                                break;
                                        }
                                    }
                                    howTo.x = 140;
								    howTo.y = 60;
								    howTo.w = 640;
								    howTo.h = 480;
                                    
                                    SDL_SetRenderDrawColor((*game).renderer, 0, 0, 0, 255);
                                    SDL_RenderClear((*game).renderer);
                                    SDL_RenderCopy((*game).renderer, Tex, NULL, &howTo);
                                    SDL_RenderPresent((*game).renderer);
                                }
                            }
                            else {
                                for (int i = 0; i< nrOptions; i++)
                                {
                                    SDL_FreeSurface(tmp[i]);
                                    SDL_DestroyTexture((*game).label[i]);
                                }
                                int test = i;
                                return i;
                            }
                            
                        }
                    }
                    break;
                    
                default:
                    break;
            }
            
        }
        SDL_SetRenderDrawColor((*game).renderer, 0, 0, 0, 255);
        SDL_RenderClear((*game).renderer);
        
        for (int i = 0; i<nrOptions; i++) {
            SDL_RenderCopy((*game).renderer, (*game).label[i], NULL, &textRect[i]);
        }
        
        SDL_RenderPresent((*game).renderer);
    }
    
    
    return 0;
    
}
