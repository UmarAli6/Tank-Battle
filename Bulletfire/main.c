#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <time.h>
#include "tank.h"
#include "bullet.h"
#define WIDTH 640
#define HEIGTH 480
#define MAX_BULLETS 1000

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
    Tank t1;
    Tank t2;
    Tank t3;
    Tank t4;
    
    //ledges
    LedgeSide ledges[100];
    
    //Images
    SDL_Texture *bild;
    SDL_Texture *brickSide;
    SDL_Texture *brickLong;
    SDL_Texture *bulletGreen;
    
    //Renderer
    SDL_Renderer *renderer;
} GameState;

void loadGame(GameState *game, int *pnrOfConnections);
void collisionDetect(GameState *game);
int processEvents(SDL_Window *window, GameState *game);
void doRender(SDL_Renderer *renderer, GameState *game, int *pnrOfConnections);
void updateLogic(GameState *game);

UDPsocket sd;
IPaddress srvadd;
UDPpacket *p;
UDPpacket *p2 = NULL;
UDPpacket *pConnections = NULL;


int main(int argc, char *argv[])
{
    char server[20];
    int port;
    int nrOfConnections = 0;
    
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    if (!(sd = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    /* Resolve server name  */
    printf("Enter server: ");
    scanf("%s",server);
    printf("Enter port: ");
    scanf("%d", &port);
    
    if (SDLNet_ResolveHost(&srvadd, server, port) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    if (!((p = SDLNet_AllocPacket(512)) && (p2 = SDLNet_AllocPacket(512))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    GameState gameState;
    SDL_Window *window;                    // Declare a window
    SDL_Renderer *renderer;                // Declare a renderer
    
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    
    //Create an application window with the following settings:
    window = SDL_CreateWindow("Game Window",                     // window title
                              SDL_WINDOWPOS_UNDEFINED,           // initial x position
                              SDL_WINDOWPOS_UNDEFINED,           // initial y position
                              WIDTH,                               // width, in pixels
                              HEIGTH,                               // height, in pixels
                              0                                  // flags
                              );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gameState.renderer = renderer;
    
    loadGame(&gameState, &nrOfConnections);
    
    // The window is open: enter program loop (see SDL_PollEvent)
    int done = 0;
    
    //Event loop
    while (!done)
    {
        //Check for events
        done = processEvents(window, &gameState);
        
        // update entity members
        updateLogic(&gameState);
        
        collisionDetect(&gameState);
        
        //Render display
        doRender(renderer, &gameState, &nrOfConnections);
        
    }
    
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (gameState.bullets[i] != NULL)
        {
            destroyBullet(&gameState.bullets[i], i);
        }
    }
    
    destroyTank(gameState.t1);
    //Shutdown game and unload all memory
    SDL_DestroyTexture(gameState.bild);
    
    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    // Clean up
    SDL_Quit();
    return 0;
}

void loadGame(GameState *game, int *pnrOfConnections)
{
    int finish=0;
    int gotPacket;
    SDL_Surface *surface = NULL;
    //Load images and create rendering textures from them
    surface = IMG_Load("tank_sand.png");
    if (surface == NULL)
    {
        printf("Could not find tank.png!\n");
        SDL_Quit();
        exit(1);
    }
    
    game->bild = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("fenceRed.png");
    game->brickSide = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("fenceRedlong.png");
    game->brickLong = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("bulletDark2.png");
    game->bulletGreen = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    char client[50]="Client";
    //SEND SOMETHING TO SERVER FOR CONNECTION
    sprintf((char *)p->data, "%s\n", client);
    p->address.host = srvadd.host;    /* Set the destination host */
    p->address.port = srvadd.port;    /* And destination port */
    p->len = strlen((char *)p->data) + 1;
    SDLNet_UDP_Send(sd, -1, p);
    
    while (!finish) {
        
        gotPacket=SDLNet_UDP_Recv(sd, p2);
        if (gotPacket) {
            sscanf((char *)p2->data,"%d\n", pnrOfConnections);
            printf("NEW CONNECTION! THERE IS %d ACTIVE CONNECTIONS\n", *pnrOfConnections);
            if (*pnrOfConnections==1) {
                
                game->t1 = createTank(0,0);
                setTankAngle(game->t1, -90);
                game->t2 = createTank(0,432);
                setTankAngle(game->t2, 90);
                game->t3 = createTank(592,0);
                setTankAngle(game->t3, -90);
                game->t4 = createTank(592,432);
                setTankAngle(game->t4, 90);
                finish=1;
            }
            if (*pnrOfConnections==2) {
                
                game->t2 = createTank(0,0);
                setTankAngle(game->t2, -90);
                game->t1 = createTank(592,0);
                setTankAngle(game->t1, -90);
                game->t3 = createTank(592,432);
                setTankAngle(game->t3, 90);
                game->t4 = createTank(0,432);
                setTankAngle(game->t4, 90);
                finish=1;
                
            }
            if (*pnrOfConnections==3) {
                
                game->t2 = createTank(592,432);
                setTankAngle(game->t2, -90);
                game->t3 = createTank(592,0);
                setTankAngle(game->t3, -90);
                game->t1 = createTank(0,432);
                setTankAngle(game->t1, 90);
                game->t4 = createTank(0,0);
                setTankAngle(game->t4, 90);
                finish=1;
            }
            if (*pnrOfConnections==4) {
                
                game->t2 = createTank(0,0);
                setTankAngle(game->t2, -90);
                game->t3 = createTank(592,0);
                setTankAngle(game->t3, -90);
                game->t4 = createTank(0,432);
                setTankAngle(game->t4, 90);
                game->t1 = createTank(592,432);
                setTankAngle(game->t1, 90);
                finish=1;
            }
            
        }
    }
    
    
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        game->bullets[i] = NULL;
    }
    /*
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
     */
}

void collisionDetect(GameState *game)
{
    //Ser till så att man håller sig innanför fönstret
    if (getTankPositionX(game->t1) <= 0)
    {
        setTankX(game->t1, 0);
    }
    if (getTankPositionY(game->t1) <= 0)
    {
        setTankY(game->t1, 0);
    }
    if (getTankPositionX(game->t1) >= 640 - 48)
    {
        setTankX(game->t1, 592);
    }
    if (getTankPositionY(game->t1) >= 480 - 48)
    {
        setTankY(game->t1, 432);
    }
    
    //Bullets within window
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (game->bullets[i]!=NULL)
        {
            {
                if (getBulletPositionX(game->bullets[i]) < -20 || getBulletPositionX(game->bullets[i]) > 700)
                {
                    destroyBullet(&game->bullets[i], i);
                }
                else if (getBulletPositionY(game->bullets[i]) < -20 || getBulletPositionY(game->bullets[i]) > 700)
                {
                    destroyBullet(&game->bullets[i], i);
                }
            }
        }
    }
    /*for (int i = 0; i < MAX_BULLETS; i++)
     {
     int mw = 10, mh = 10;
     int mx = getBulletPositionX(&game->bullets[i]), my = getBulletPositionY(&game->bullets[i]);
     int bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;
     
     
     if (my + mh > by &&  my < by + bh)
     {
     //rubbing against right edge
     if (mx < bx + bw  &&  mx + mw > bx + bw)
     {
     //correct x
     destroyBullet(&game->bullets[i], i);
     }
     //rubbing against left edge
     else if (mx + mw > bx && mx < bx)
     {
     //correct x
     destroyBullet(&game->bullets[i], i);
     }
     }
     
     if (mx + mh > bx && mx < bx + bw)
     {
     //are we bumping our head
     if (my < by + bh && my > by)
     {
     //correct y
     destroyBullet(&game->bullets[i], i);
     
     }
     //are we landing on the ledge
     else if (my + mh > by && my < by)
     {
     //correct y
     destroyBullet(&game->bullets[i], i);
     }
     }
     }
     */
    
    //Check for collision with any ledges (brick blocks)
    
}

int processEvents(SDL_Window *window, GameState *game)
{
    SDL_Event event;
    int done = 0;
    unsigned int currentTime;
    
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
        setTankAngleOLD(game->t1, getTankAngle(game->t1));
        updateTankAngle(game->t1, -3);
        if (getTankAngle(game->t1) < 0)
        {
            setTankAngle(game->t1, 360);
        }
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        setTankAngleOLD(game->t1, getTankAngle(game->t1));
        updateTankAngle(game->t1, 3);
        if (getTankAngle(game->t1) > 359)
        {
            setTankAngle(game->t1, 0);
        }
    }
    if (state[SDL_SCANCODE_SPACE])
    {
        currentTime = SDL_GetTicks();
        if (currentTime > getTimer(game->t1) + 500)
        {
            int found = -1;
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (game->bullets[i] == NULL)
                {
                    found = i;
                    break;
                }
            }
            if (found >= 0)
            {
                game->bullets[found] =createBullet(getTankPositionX(game->t1), getTankPositionY(game->t1), sin(getTankRadian(game->t1)), cos(getTankRadian(game->t1)), getTankAngle(game->t1));
                setTimer(game->t1, currentTime);
            }
        }
    }
    if (state[SDL_SCANCODE_UP])
    {
        setTankVelocity(game->t1, -3);
    }
    else if (state[SDL_SCANCODE_DOWN])
    {
        setTankVelocity(game->t1, 3);
    }
    else
    {
        setTankVelocity(game->t1, 0);
    }
    
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (game->bullets[i] != NULL)
        {
            printf("%0.1f        %0.1f\n", getBulletPositionX(game->bullets[i]), getBulletPositionY(game->bullets[i]));
        }
    }
    
    return done;
}

void updateLogic(GameState *game)
{
    float xOLD, xNEW, yOLD, yNEW, angleOLD, angleNEW;
    setTankRadian(game->t1, getTankAngle(game->t1) * (M_PI / 180.0));
    setTankDx(game->t1, cos(getTankRadian(game->t1)) * getTankVelocity(game->t1));
    setTankDy(game->t1, sin(getTankRadian(game->t1)) * getTankVelocity(game->t1));
    xOLD=getTankPositionX(game->t1);
    yOLD=getTankPositionY(game->t1);
    angleOLD=getTankAngleOLD(game->t1);
    
    updateTankPositionX(game->t1, getTankDx(game->t1));
    updateTankPositionY(game->t1, getTankDy(game->t1));
    setTankAngleOLD(game->t1, getTankAngle(game->t1));
    
    xNEW=getTankPositionX(game->t1);
    yNEW=getTankPositionY(game->t1);
    angleNEW = getTankAngle(game->t1);
    
    
    for (int i = 0; i < MAX_BULLETS; i++) if (game->bullets[i])
    {
        updateBulletPositionX(game->bullets[i], getBulletDx(game->bullets[i]));
        updateBulletPositionY(game->bullets[i], getBulletDy(game->bullets[i]));
    }
    
    // printf("OLD: %0.1f\n", getTankAngleOLD(game->t1));
    
    if (xOLD != xNEW || yOLD != yNEW || angleOLD != angleNEW) {
        
        sprintf((char *)p->data, "%f %f %f\n", xNEW, yNEW, angleNEW);
        p->address.host = srvadd.host;    /* Set the destination host */
        p->address.port = srvadd.port;    /* And destination port */
        p->len = strlen((char *)p->data) + 1;
        SDLNet_UDP_Send(sd, -1, p);
    }
    
}

void doRender(SDL_Renderer *renderer, GameState *game, int *pnrOfConnections)
{
    float xCORD, yCORD, dAngle;
    int c;
    if (SDLNet_UDP_Recv(sd, p2)) {
        sscanf((char *)p2->data, "%f %f %d %f %d\n", &xCORD, &yCORD, &c, &dAngle, pnrOfConnections);
        if (c==1) {
            setEnemyX(game->t2, xCORD);
            setEnemyY(game->t2, yCORD);
            setEnemyAngle(game->t2, dAngle);
        }
        if (c==2) {
            
            setEnemyX(game->t3, xCORD);
            setEnemyY(game->t3, yCORD);
            setEnemyAngle(game->t3, dAngle);
        }
        if (c==3) {
            
            setEnemyX(game->t4, xCORD);
            setEnemyY(game->t4, yCORD);
            setEnemyAngle(game->t4, dAngle);
        }
        
    }
    //set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    //Clear the screen (to blue)
    SDL_RenderClear(renderer);
    
    //set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for (int i = 0; i<49; i++)
    {
        SDL_Rect ledgeRect = { game->ledges[i].x,game->ledges[i].y,game->ledges[i].w,game->ledges[i].h };
        SDL_RenderCopy(renderer, game->brickSide, NULL, &ledgeRect);
    }
    for (int i = 49; i<100; i++)
    {
        SDL_Rect ledgeRect = { game->ledges[i].x,game->ledges[i].y,game->ledges[i].w,game->ledges[i].h };
        SDL_RenderCopy(renderer, game->brickLong, NULL, &ledgeRect);
    }
    
    for (int i = 0; i < MAX_BULLETS; i++) if (game->bullets[i])
    {
        SDL_Rect bullrect = { getBulletPositionX(game->bullets[i]) + 20, getBulletPositionY(game->bullets[i]) + 24, 10, 10 };
        SDL_RenderCopyEx(renderer, game->bulletGreen, NULL, &bullrect, getBulletAngle(game->bullets[i]) + 90, NULL, SDL_FLIP_VERTICAL);
    }
    SDL_Rect rect = { getTankPositionX(game->t1), getTankPositionY(game->t1),48,48 };
    SDL_Rect secondDest = {getTankPositionX(game->t2), getTankPositionY(game->t2) ,48,48 };
    SDL_Rect thirdDest = {getTankPositionX(game->t3), getTankPositionY(game->t3) ,48,48 };
    SDL_Rect fourthDest = {getTankPositionX(game->t4), getTankPositionY(game->t4) ,48,48 };
    
    SDL_RenderCopyEx(renderer, game->bild, NULL, &rect, getTankAngle(game->t1)+90, NULL, 0);
    SDL_RenderCopyEx(renderer, game->bild, NULL, &secondDest, getTankAngle(game->t2)+90, NULL, 0);
    SDL_RenderCopyEx(renderer, game->bild, NULL, &thirdDest, getTankAngle(game->t3)+90, NULL, 0);
    SDL_RenderCopyEx(renderer, game->bild, NULL, &fourthDest, getTankAngle(game->t4)+90, NULL, 0);

    
    
    
    
    
    //We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(renderer);
}
