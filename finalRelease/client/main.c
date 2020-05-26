#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_ttf.h>
#include "tank.h"
#include "bullet.h"
#include "game.h"
#include "network.h"

#define WIDTH 960
#define HEIGTH 720

int main(int argc, char *argv[])
{
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket *p;
    UDPpacket *p2;
    
    int nrOfConnections = 0;
    
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        getchar();
        exit(EXIT_FAILURE);
    }
    
    if (!(sd = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        getchar();
        exit(EXIT_FAILURE);
    }
    
    if (!((p = SDLNet_AllocPacket(512)) && (p2 = SDLNet_AllocPacket(512))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        getchar();
        exit(EXIT_FAILURE);
    }
    
    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    
    
    GameState gameState;
    SDL_Window *window;                    // Declare a window
    SDL_Renderer *renderer;                // Declare a renderer
    
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    
    //Create an application window with the following settings:
    window = SDL_CreateWindow("Free-For-All: First to score 3 wins | Score: You:0, Player2:0, Player3:0, Player4:0", // window title
                              SDL_WINDOWPOS_UNDEFINED,           // initial x position
                              SDL_WINDOWPOS_UNDEFINED,           // initial y position
                              WIDTH,                               // width, in pixels
                              HEIGTH,                               // height, in pixels
                              0                                  // flags
                              );
    int which;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gameState.renderer = renderer;
    
    
    
    // The window is open: enter program loop (see SDL_PollEvent)
    bool gameOver = false;
    
    //Event loop
    while (!gameOver)
    {
        int done = 0;
        int test = showMenu(&gameState);
        
        if (test!=0) {
            gameOver=true;
        }
        else {
            
            while (!setupConnection(&sd, &srvadd, p, p2, &nrOfConnections)) {}
            loadGame(&gameState, &nrOfConnections, &sd, &srvadd, p, p2, &which);
            
            while (done==0) {
                
                //Check for events
                gameOver = processEvents(window, &gameState, &sd, &srvadd, p, &nrOfConnections);
                if (gameOver == 1) {
                    done=1;
                }
                //Update entity members
                updateLogic(&gameState, &sd, &srvadd, p);
                
                //Check for Collision
                collisionDetect(&gameState);
                
                //Render display
                doRender(renderer, &gameState, &nrOfConnections, &sd, p2, &which);
                
                //Check if game is done
                handleScore(&gameState, &done, window, &sd, &srvadd, p);
                if (done==1) {
                    char startOver[20]="Reset";
                    sprintf((char *)p->data, "%s\n", startOver);
                    p->address.host = srvadd.host;    /* Set the destination host */
                    p->address.port = srvadd.port;    /* And destination port */
                    p->len = strlen((char *)p->data) + 1;
                    SDLNet_UDP_Send(sd, -1, p);
                }
            }
            
        }
        
    }
    
   
    
    //Shutdown game and unload all memory
    SDL_DestroyTexture(gameState.tankSandTexture);
    SDL_DestroyTexture(gameState.tankDarkTexture);
    SDL_DestroyTexture(gameState.tankGreenTexture);
    SDL_DestroyTexture(gameState.tankBlueTexture);
    SDL_DestroyTexture(gameState.tankSandTextureDead);
    SDL_DestroyTexture(gameState.tankDarkTextureDead);
    SDL_DestroyTexture(gameState.tankGreenTextureDead);
    SDL_DestroyTexture(gameState.tankBlueTextureDead);
    SDL_DestroyTexture(gameState.bullet);
    
    destroyTank(gameState.tanks[0]);
    destroyTank(gameState.tanks[1]);
    destroyTank(gameState.tanks[2]);
    destroyTank(gameState.tanks[3]);
    
    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    // Clean up
    TTF_Quit();
    SDL_Quit();
    return 0;
}
