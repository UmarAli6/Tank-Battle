#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

const int WIDTH = 640, HEIGTH=480;
bool init();
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gBack = NULL;
SDL_Texture* gSingle = NULL;
SDL_Texture* gMulti = NULL;
SDL_Texture* gQuit = NULL;
SDL_Rect background_rect, Single_rect, Multi_rect, Quit_rect;

int Mx = 0;
int My = 0;
bool play1 = false;

int main(int argc, char *argv[])
{
    
    if (init()) {
        printf("worked\n");
    }
    background_rect.x = 0;   //X COORDINATE
    background_rect.y = 0;   //Y COORDINATE
    background_rect.w = 640; //WIDTH
    background_rect.h = 480; //HEIGHT
    
    Single_rect.w = 200;
    Single_rect.h = 100;
    Single_rect.x = WIDTH/2-Single_rect.w/2;
    Single_rect.y = 25;
    
    Multi_rect.w = 200;
    Multi_rect.h = 100;
    Multi_rect.x = WIDTH/2-Multi_rect.w/2;
    Multi_rect.y = Single_rect.y + Single_rect.h + 25;
    
    Quit_rect.w = 200;
    Quit_rect.h = 100;
    Quit_rect.x = WIDTH/2-Quit_rect.w/2;
    Quit_rect.y = Multi_rect.y + Multi_rect.h + 25;
    
    
    SDL_Event windowEvent;
    int quit=0;
    
    while(!quit) {
        
        SDL_GetMouseState(&Mx, &My);
        
        if (Mx >= Single_rect.x && Mx <= Single_rect.x + Single_rect.w && My >= Single_rect.y && My <= Single_rect.y + Single_rect.h){
            
            SDL_SetTextureColorMod(gSingle, 250, 0, 0 );
        }
        else
        {
            SDL_SetTextureColorMod(gSingle, 250, 250, 250);
        }
        if (Mx >= Multi_rect.x && Mx <= Multi_rect.x + Multi_rect.w && My >= Multi_rect.y && My <= Multi_rect.y + Multi_rect.h){
            
            SDL_SetTextureColorMod(gMulti, 250, 0, 0 );
        }
        else
        {
            SDL_SetTextureColorMod(gMulti, 250, 250, 250);
        }
        if (Mx >= Quit_rect.x && Mx <= Quit_rect.x + Quit_rect.w && My >= Quit_rect.y && My <= Quit_rect.y + Quit_rect.h){
            
            SDL_SetTextureColorMod(gQuit, 250, 0, 0 );
        }
        else
        {
            SDL_SetTextureColorMod(gQuit, 250, 250, 250);
        }
        while(SDL_PollEvent(&windowEvent)){
            switch (windowEvent.type){
                case SDL_QUIT:
                    quit=1;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (windowEvent.button.button == SDL_BUTTON_LEFT)
                    {
                        
                        if(Mx >= Single_rect.x && Mx <= Single_rect.x + Single_rect.w && My >= Single_rect.y && My <= Single_rect.y + Single_rect.h)
                        {
                            printf("Single");
                        }
                        else if (Mx >= Multi_rect.x && Mx <= Multi_rect.x + Multi_rect.w && My >= Multi_rect.y && My <= Multi_rect.y + Multi_rect.h)
                        {
                            printf("Multi");
                        }
                        else if (Mx >= Quit_rect.x && Mx <= Quit_rect.x + Quit_rect.w && My >= Quit_rect.y && My <= Quit_rect.y + Quit_rect.h)
                        {
                            printf("Quit");
                        }
                        
                    }
            
                default:
                    break;
            }
        
        }
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, gBack, NULL, &background_rect);
        SDL_RenderCopy(gRenderer, gSingle, NULL, &Single_rect);
        SDL_RenderCopy(gRenderer, gMulti, NULL, &Multi_rect);
        SDL_RenderCopy(gRenderer, gQuit, NULL, &Quit_rect);
        SDL_RenderPresent(gRenderer);
        
        if (play1 == true) {
            printf("Fungerar");
            play1=false;
        }
        }
    
    
         return EXIT_SUCCESS;
    }

bool init(){
    bool test = true;
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("TANKS", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if(gWindow == NULL){
        printf("Fungerar ej WINDOW\n");
        test = false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
    if(gRenderer == NULL){
        printf("Fungerar ej RENDERER\n");
        test = false;
    }
    SDL_Surface* mBack = IMG_Load("Background.png");
    gBack = SDL_CreateTextureFromSurface(gRenderer, mBack);
    
    SDL_Surface* mSingle = IMG_Load("Singlebutton.png");
    gSingle = SDL_CreateTextureFromSurface(gRenderer, mSingle);
    
    SDL_Surface* mMulti = IMG_Load("MultiButton.png");
    gMulti = SDL_CreateTextureFromSurface(gRenderer, mMulti);
    
    SDL_Surface* mQuit = IMG_Load("QuitButton.png");
    gQuit = SDL_CreateTextureFromSurface(gRenderer, mQuit);

    SDL_FreeSurface(mQuit);
    SDL_FreeSurface(mMulti);
    SDL_FreeSurface(mBack);
    SDL_FreeSurface(mSingle);
    return test;
    
}
