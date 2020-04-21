#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "MainMenu.h"
#include "GameState.h"

const int WIDTH = 640, HEIGTH=480;
bool init();
void loadGame(GameState *game);
void collisionDetect(GameState *game);
int processEvents(SDL_Window *window, GameState *game);
void doRender(SDL_Renderer *renderer, GameState *game);
SDL_Window* gameWindow = NULL;
SDL_Renderer* gameRenderer = NULL;
SDL_Window* menuWindow = NULL;
SDL_Renderer* menuRenderer = NULL;
SDL_Texture* menuBack = NULL;
SDL_Texture* menuSingle = NULL;
SDL_Texture* menuMulti = NULL;
SDL_Texture* menuQuit = NULL;
SDL_Rect background_rect, Single_rect, Multi_rect, Quit_rect;

int Mx = 0;
int My = 0;
bool play1 = false;

int main(int argc, char *argv[])
{
    GameState gameState;
    
    if (init()) {
        printf("worked\n");
    }
    
    
    background_rect.x = 0;
    background_rect.y = 0;
    background_rect.w = 640;
    background_rect.h = 480;
    
    Single_rect.w = 200;
    Single_rect.h = 100;
    Single_rect.x = WIDTH/2-Single_rect.w/2;
    Single_rect.y = 70;
    
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
        
        previewChoice(Mx, My, Single_rect, menuSingle);
        previewChoice(Mx, My, Multi_rect, menuMulti);
        previewChoice(Mx, My, Quit_rect, menuQuit);
        
        while(SDL_PollEvent(&windowEvent)){
            switch (windowEvent.type){
                case SDL_QUIT:
                    quit=1;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (windowEvent.button.button == SDL_BUTTON_LEFT)
                    {
                        if(previewChoice(Mx, My, Single_rect, menuSingle))
                        {
                            //Create an application window with the following settings:
                            gameWindow = SDL_CreateWindow("Game Window",                     // window title
                                                      SDL_WINDOWPOS_UNDEFINED,           // initial x position
                                                      SDL_WINDOWPOS_UNDEFINED,           // initial y position
                                                      WIDTH,                               // width, in pixels
                                                      HEIGTH,                               // height, in pixels
                                                      0                                  // flags
                                                      );
                            gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                            gameState.renderer = gameRenderer;
                            
                            SDL_DestroyTexture(menuBack);
                            SDL_DestroyTexture(menuSingle);
                            SDL_DestroyTexture(menuMulti);
                            SDL_DestroyTexture(menuQuit);
                            SDL_DestroyRenderer(menuRenderer);
                            SDL_DestroyWindow(menuWindow);
                            
                            loadGame(&gameState);
                            
                            int done = 0;
                            
                            //Event loop
                            while (!done)
                            {
                                //Check for events
                                done = processEvents(gameWindow, &gameState);
                                
                                collisionDetect(&gameState);
                                
                                //Render display
                                doRender(gameRenderer, &gameState);
                                
                            }
                            
                            //Shutdown game and unload all memory
                            SDL_DestroyTexture(gameState.tank);
                            
                            // Close and destroy the window
                            SDL_DestroyWindow(gameWindow);
                            SDL_DestroyRenderer(gameRenderer);
                            
                            // Clean up
                            quit=1;
                            SDL_Quit();
                           
                        }
                        else if (previewChoice(Mx, My, Multi_rect, menuMulti))
                        {
                            printf("Multi");
                        }
                        else if (previewChoice(Mx, My, Quit_rect, menuQuit))
                        {
                            printf("Quit");
                            quit=1;
                        }
                    }
            
                default:
                    break;
            }
        
        }
        SDL_RenderClear(menuRenderer);
        SDL_RenderCopy(menuRenderer, menuBack, NULL, &background_rect);
        SDL_RenderCopy(menuRenderer, menuSingle, NULL, &Single_rect);
        SDL_RenderCopy(menuRenderer, menuMulti, NULL, &Multi_rect);
        SDL_RenderCopy(menuRenderer, menuQuit, NULL, &Quit_rect);
        SDL_RenderPresent(menuRenderer);
        
        if (play1 == true) {
            printf("Fungerar");
            play1=false;
        }
    }
    
    
        SDL_Quit();
    
         return EXIT_SUCCESS;
    }


bool init(){
    bool test = true;
    SDL_Init(SDL_INIT_VIDEO);
    menuWindow = SDL_CreateWindow("TANKS", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if(menuWindow == NULL){
        printf("Fungerar ej WINDOW\n");
        test = false;
    }
    menuRenderer = SDL_CreateRenderer(menuWindow, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
    if(menuRenderer == NULL){
        printf("Fungerar ej RENDERER\n");
        test = false;
    }
    SDL_Surface* mBack = IMG_Load("Background.png");
    menuBack = SDL_CreateTextureFromSurface(menuRenderer, mBack);
    
    SDL_Surface* mSingle = IMG_Load("Singlebutton.png");
    menuSingle = SDL_CreateTextureFromSurface(menuRenderer, mSingle);
    
    SDL_Surface* mMulti = IMG_Load("MultiButton.png");
    menuMulti = SDL_CreateTextureFromSurface(menuRenderer, mMulti);
    
    SDL_Surface* mQuit = IMG_Load("QuitButton.png");
    menuQuit = SDL_CreateTextureFromSurface(menuRenderer, mQuit);

    SDL_FreeSurface(mQuit);
    SDL_FreeSurface(mMulti);
    SDL_FreeSurface(mBack);
    SDL_FreeSurface(mSingle);
    return test;
    
}

void loadGame(GameState *game)
{
    SDL_Surface *surface = NULL;
    //Load images and create rendering textures from them
    surface = IMG_Load("tank_green.png");
    if (surface == NULL)
    {
        printf("Could not find tank.png!\n");
        SDL_Quit();
        exit(1);
    }
    
    game->tank = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("fenceRed.png");
    game->brickSide = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("fenceRedlong.png");
    game->brickLong = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    initGame(game);
}

void collisionDetect(GameState *game)
{
    //Ser till så att man håller sig innanför fönstret
    if (game->man.x <= 0) game->man.x = 0;
    if (game->man.y <= 0) game->man.y = 0;
    if (game->man.x >= 640 - 48 ) game->man.x = 640 - 48;
    if (game->man.y >= 480- 48) game->man.y = 480 - 48;
    //Check for collision with any ledges (brick blocks)
    for (int i = 0; i < 100; i++)
    {
        float mw = 48, mh = 48;
        float mx = game->man.x, my = game->man.y;
        float bx = game->ledges[i].x,by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;
        
        if (my + mh > by &&  my < by + bh)
        {
            //rubbing against right edge
            if (mx < bx + bw  &&  mx + mw > bx + bw)
            {
                //correct x
                game->man.x = bx+bw;
                mx = bx+bw;
            }
            //rubbing against left edge
            else if(mx+mw > bx && mx < bx)
            {
                //correct x
                game->man.x = bx-mw;
                mx = bx-mw;
            }
        }
        
        if (mx + mw > bx && mx < bx + bw)
        {
            //are we bumping our head
            if (my < by + bh && my > by)
            {
                //correct y
                game->man.y = by+ bh;
                
            }
            //are we landing on the ledge
            else if(my+ mh > by && my < by)
            {
                //correct y
                game->man.y = by-mh;
                
            }
        }
    }
}

int processEvents(SDL_Window *window, GameState *game)
{
    SDL_Event event;
    int done = 0;
    
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if (window)
                {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = 1;
                }
            }
                break;
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        done = 1;
                        break;
                }
            }
                break;
            case SDL_QUIT:
                //quit out of the game
                done = 1;
                break;
        }
    }
    
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT])
    {
        game->man.angle -= 5.0;
        if (game->man.angle < 0.0) game->man.angle = 360.0;
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        game->man.angle += 5.0;
        if (game->man.angle > 359.0) game->man.angle = 0.0;
    }
    if (state[SDL_SCANCODE_UP])
    {
        game->man.vel = -3.0;
    }
    else if (state[SDL_SCANCODE_DOWN])
    {
        game->man.vel = 3.0;
    }
    else game->man.vel = 0.0;
    
    game->man.rad = game->man.angle * (M_PI / 180.0);
    game->man.dx = cos(game->man.rad) * game->man.vel;
    game->man.dy = sin(game->man.rad) * game->man.vel;
    game->man.x += game->man.dx;
    game->man.y += game->man.dy;
    
    return done;
}

void doRender(SDL_Renderer *renderer, GameState *game)
{
    //set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    
    //Clear the screen (to blue)
    SDL_RenderClear(renderer);
    
    //set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for(int i=0;i<49;i++)
    {
        SDL_Rect ledgeRect = {game->ledges[i].x,game->ledges[i].y,game->ledges[i].w,game->ledges[i].h};
        SDL_RenderCopy(renderer,game->brickSide,NULL,&ledgeRect);
    }
    for (int i = 49;i<100;i++)
    {
        SDL_Rect ledgeRect = { game->ledges[i].x,game->ledges[i].y,game->ledges[i].w,game->ledges[i].h };
        SDL_RenderCopy(renderer, game->brickLong, NULL, &ledgeRect);
    }
    
    SDL_Rect rect = { game->man.x, game->man.y,48,48};
    SDL_RenderCopyEx(renderer, game->tank, NULL, &rect, game->man.angle + 90, NULL, 0);
    
    //We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(renderer);
    
}


