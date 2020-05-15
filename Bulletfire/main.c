#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include "SDL2/SDL_ttf.h"
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
    SDL_Texture *bild2;
    SDL_Texture *bild3;
    SDL_Texture *bild4;
    SDL_Texture *brickSide;
    SDL_Texture *brickLong;
    SDL_Texture *bulletGreen;
    SDL_Texture *label[3];
    SDL_Texture *waitFlag;
    
    //Fonts
    TTF_Font *font;
    
    //Renderer
    SDL_Renderer *renderer;
} GameState;

int loadGame(GameState *game, int *pnrOfConnections, int *pwhich);
void collisionDetect(GameState *game);
int processEvents(SDL_Window *window, GameState *game, int *pnrOfConnections);
void doRender(SDL_Renderer *renderer, GameState *game, int *pnrOfConnections, int *pwhich);
void updateLogic(GameState *game);
int showMenu(GameState *game);

UDPsocket sd;
IPaddress srvadd;
UDPpacket *p;
UDPpacket *p2 = NULL;
UDPpacket *pConnections = NULL;

SDL_Surface* surface;
TTF_Font* font;


int main(int argc, char *argv[])
{
   
    int nrOfConnections = 0;
    int which = 0;
    
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
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
    window = SDL_CreateWindow("Game Window",                     // window title
                              SDL_WINDOWPOS_UNDEFINED,           // initial x position
                              SDL_WINDOWPOS_UNDEFINED,           // initial y position
                              WIDTH,                               // width, in pixels
                              HEIGTH,                               // height, in pixels
                              0                                  // flags
                              );
    
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gameState.renderer = renderer;

    bool done = false;
    int test = showMenu(&gameState);
    if (test!=0) {
        done=true;
    }
    else
    {
            if (!(sd = SDLNet_UDP_Open(0)))
            {
                fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
                exit(EXIT_FAILURE);
            }
            
            /* Resolve server name  */
            
            if (SDLNet_ResolveHost(&srvadd, "127.0.0.1", 2000) == -1)
            {
                fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
                exit(EXIT_FAILURE);
            }
            
            if (!((p = SDLNet_AllocPacket(512)) && (p2 = SDLNet_AllocPacket(512))))
            {
                fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
                exit(EXIT_FAILURE);
            }
            
            done = loadGame(&gameState, &nrOfConnections, &which);
        
    }
    
    //Event loop
    while (!done)
    {
        //Check for events
        done = processEvents(window, &gameState, &nrOfConnections);
        
        // update entity members
        updateLogic(&gameState);
        
        collisionDetect(&gameState);
        
        //Render display
        doRender(renderer, &gameState, &nrOfConnections, &which);
        
    }
    
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (gameState.bullets[i] != NULL)
        {
            destroyBullet(gameState.bullets[i], i);
            gameState.bullets[i] = NULL;
        }
    }
    
    destroyTank(gameState.t1);
    destroyTank(gameState.t2);
    destroyTank(gameState.t3);
    destroyTank(gameState.t4);
    //Shutdown game and unload all memory
    SDL_DestroyTexture(gameState.bild);
    SDL_DestroyTexture(gameState.bild2);
    SDL_DestroyTexture(gameState.bild3);
    SDL_DestroyTexture(gameState.bild4);
    
    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    TTF_Quit();
    // Clean up
    SDL_Quit();
    return 0;
}

void shutDown(GameState* game)
{
    
}

int showMenu(GameState *game)
{
    game->font = TTF_OpenFont("/Library/fonts/chalkduster.ttf", 30);
    if (!game->font) {
        printf("Couldnt load font!!\n");
        SDL_Quit();
        exit(1);
    }
    
    SDL_Color textColor[2] = {{255,255,255},{255,0,0}};
    const int nrOptions = 3;
    int x, y;
    SDL_Surface *tmp[nrOptions];
    tmp[0]= TTF_RenderText_Blended(game->font, "Start game", textColor[0]);
    game->label[0] = SDL_CreateTextureFromSurface(game->renderer, tmp[0]);
    
    tmp[1] = TTF_RenderText_Blended(game->font, "How to play", textColor[0]);
    game->label[1] = SDL_CreateTextureFromSurface(game->renderer, tmp[1]);
    
    tmp[2] = TTF_RenderText_Blended(game->font, "Exit", textColor[0]);
    game->label[2] = SDL_CreateTextureFromSurface(game->renderer, tmp[2]);
    
    
    SDL_Rect textRect[nrOptions];
    SDL_Rect howTo;
    textRect[0].x= 320-tmp[0]->w/2;
    textRect[0].y= 175;
    textRect[0].w= tmp[0]->w;
    textRect[0].h= tmp[0]->h;
    
    textRect[1].x= 320-tmp[1]->w/2;
    textRect[1].y= 195+tmp[0]->h;
    textRect[1].w= tmp[1]->w;
    textRect[1].h= tmp[1]->h;
    
    textRect[2].x= 320-tmp[2]->w/2;
    textRect[2].y= 215+tmp[0]->h+tmp[1]->h;
    textRect[2].w= tmp[2]->w;
    textRect[2].h= tmp[2]->h;
    
    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    for(int i = 0; i< nrOptions; i++)
                    {
                        SDL_FreeSurface(tmp[i]);
                        SDL_DestroyTexture(game->label[i]);
                    }
                    return 1;
                    break;
                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    for(int i = 0; i < nrOptions; i++) {
                        if(x>=textRect[i].x && x<=textRect[i].x+textRect[i].w && y>=textRect[i].y && y<=textRect[i].y+textRect[i].h)
                        {
                            SDL_SetTextureColorMod(game->label[i], 250, 0, 0 );
                        }
                        else
                        {
                            SDL_SetTextureColorMod(game->label[i], 255, 255, 255 );
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x;
                    y = event.button.y;
                    for(int i = 0; i < nrOptions; i++) {
                        if(x>=textRect[i].x && x<=textRect[i].x+textRect[i].w && y>=textRect[i].y && y<=textRect[i].y+textRect[i].h)
                        {
                            if (x>=textRect[1].x && x<=textRect[1].x+textRect[1].w && y>=textRect[1].y && y<=textRect[1].y+textRect[1].h) {
                                
                                SDL_Surface *test;
                                test = IMG_Load("tutorial.png");
                                SDL_Texture *Tex = SDL_CreateTextureFromSurface(game->renderer, test);
                                int quit=0;
                                while (!quit) {
                                    while (SDL_PollEvent(&event)) {
                                        switch (event.type) {
                                            case SDL_QUIT:
                                                for(int i = 0; i< nrOptions; i++)
                                                {
                                                    SDL_FreeSurface(tmp[i]);
                                                    SDL_DestroyTexture(game->label[i]);
                                                }
                                                return 1;
                                                
                                            case SDL_MOUSEBUTTONDOWN:
                                                SDL_FreeSurface(test);
                                                SDL_DestroyTexture(Tex);
                                                quit=1;
                                                break;
                                                
                                            default:
                                                break;
                                        }
                                    }
                                    howTo.x= 0;
                                    howTo.y= 0;
                                    howTo.w= 640;
                                    howTo.h= 480;
                                    
                                    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
                                    SDL_RenderClear(game->renderer);
                                    SDL_RenderCopy(game->renderer, Tex, NULL, &howTo);
                                    SDL_RenderPresent(game->renderer);
                                }
                            }
                            else{
                                for(int i = 0; i< nrOptions; i++)
                                {
                                    SDL_FreeSurface(tmp[i]);
                                    SDL_DestroyTexture(game->label[i]);
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
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        
        for (int i = 0; i<nrOptions; i++) {
            SDL_RenderCopy(game->renderer, game->label[i], NULL, &textRect[i]);
        }
        
        SDL_RenderPresent(game->renderer);
    }
    

    return 0;
    
}


int loadGame(GameState *game, int *pnrOfConnections, int *pwhich)
{
    
    int finish = 0;
    int gotPacket;
    SDL_Surface *surface = NULL;
    //Load images and create rendering textures from them
    
    surface = IMG_Load("fenceRed.png");
    game->brickSide = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("fenceRedlong.png");
    game->brickLong = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("bulletDark2.png");
    game->bulletGreen = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    char client[50] = "Client";
    //SEND SOMETHING TO SERVER FOR CONNECTION
    sprintf((char *)p->data, "%s\n", client);
    p->address.host = srvadd.host;    /* Set the destination host */
    p->address.port = srvadd.port;    /* And destination port */
    p->len = strlen((char *)p->data) + 1;
    SDLNet_UDP_Send(sd, -1, p);
    
    while (!finish) {
        gotPacket = SDLNet_UDP_Recv(sd, p2);
        if (gotPacket) {
            sscanf((char *)p2->data, "%d\n", pnrOfConnections);
            
            if (*pnrOfConnections == 1)
            {
                printf("NEW CONNECTION! THERE IS %d ACTIVE CONNECTIONS\n", *pnrOfConnections);
                surface = IMG_Load("tank_sand.png");
                if (surface == NULL)
                {
                    printf("Could not find tank.png!\n");
                    SDL_Quit();
                    exit(1);
                }
                
                game->bild = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_green.png");
                game->bild2 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_dark.png");
                game->bild3 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_blue.png");
                game->bild4 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                game->t1 = createTank(0, 0); //TANK SAND
                setTankAngle(game->t1, -90);
                game->t2 = createTank(0, 432); //TANK DARK
                setTankAngle(game->t2, 90);
                game->t3 = createTank(592, 0); //TANK GREEN
                setTankAngle(game->t3, -90);
                game->t4 = createTank(592, 432); //TANK BLUE
                setTankAngle(game->t4, 90);
                finish = 1;
                *pwhich = 1;
                
            }
            if (*pnrOfConnections == 2)
            {
                printf("NEW CONNECTION! THERE IS %d ACTIVE CONNECTIONS\n", *pnrOfConnections);
                surface = IMG_Load("tank_sand.png");
                if (surface == NULL)
                {
                    printf("Could not find tank.png!\n");
                    SDL_Quit();
                    exit(1);
                }
                
                game->bild = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_green.png");
                game->bild2 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_dark.png");
                game->bild3 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_blue.png");
                game->bild4 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                
                game->t2 = createTank(0, 0); //TANK SAND
                setTankAngle(game->t2, -90);
                game->t1 = createTank(592, 0); //TANK GREEN
                setTankAngle(game->t1, -90);
                game->t3 = createTank(592, 432); //TANK BLUE
                setTankAngle(game->t3, 90);
                game->t4 = createTank(0, 432); //TANK DARK
                setTankAngle(game->t4, 90);
                finish = 1;
                *pwhich = 2;
                
            }
            if (*pnrOfConnections == 3)
            {
                printf("NEW CONNECTION! THERE IS %d ACTIVE CONNECTIONS\n", *pnrOfConnections);
                surface = IMG_Load("tank_sand.png");
                if (surface == NULL)
                {
                    printf("Could not find tank.png!\n");
                    SDL_Quit();
                    exit(1);
                }
                
                game->bild = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_green.png");
                game->bild2 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_dark.png");
                game->bild3 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_blue.png");
                game->bild4 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                game->t2 = createTank(592, 432); //TANK BLUE
                setTankAngle(game->t2, 90);
                game->t3 = createTank(0, 0); //TANK SAND
                setTankAngle(game->t3, -90);
                game->t1 = createTank(0, 432); //TANK DARK
                setTankAngle(game->t1, 90);
                game->t4 = createTank(592, 0); //TANK GREEN
                setTankAngle(game->t4, -90);
                finish = 1;
                *pwhich = 3;
               
            }
            if (*pnrOfConnections == 4)
            {
                printf("NEW CONNECTION! THERE IS %d ACTIVE CONNECTIONS\n", *pnrOfConnections);
                surface = IMG_Load("tank_sand.png");
                if (surface == NULL)
                {
                    printf("Could not find tank.png!\n");
                    SDL_Quit();
                    exit(1);
                }
                
                game->bild = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_green.png");
                game->bild2 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_dark.png");
                game->bild3 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                surface = IMG_Load("tank_blue.png");
                game->bild4 = SDL_CreateTextureFromSurface(game->renderer, surface);
                SDL_FreeSurface(surface);
                
                game->t2 = createTank(592, 0); //TANK GREEN
                setTankAngle(game->t2, -90);
                game->t3 = createTank(0, 432); //TANK DARK
                setTankAngle(game->t3, 90);
                game->t4 = createTank(0, 0); //TANK SAND
                setTankAngle(game->t4, -90);
                game->t1 = createTank(592, 432); //TANK BLUE
                setTankAngle(game->t1, 90);
                finish = 1;
                *pwhich = 4;
                
            }
            
            
            if (*pnrOfConnections == 5)
            {
                printf("Server is Full! Please try again later!\n");
                return 1;
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
    
    return 0;
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
        if (game->bullets[i] != NULL)
        {
            {
                if (getBulletPositionX(game->bullets[i]) < -20 || getBulletPositionX(game->bullets[i]) > 700)
                {
                    destroyBullet(game->bullets[i], i);
                    game->bullets[i] = NULL;
                }
                else if (getBulletPositionY(game->bullets[i]) < -20 || getBulletPositionY(game->bullets[i]) > 700)
                {
                    destroyBullet(game->bullets[i], i);
                    game->bullets[i] = NULL;
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

int processEvents(SDL_Window *window, GameState *game, int *pnrOfConnections)
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
    if (*pnrOfConnections>3) {
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
                    game->bullets[found] = createBullet(getTankPositionX(game->t1), getTankPositionY(game->t1), sin(getTankRadian(game->t1)), cos(getTankRadian(game->t1)), getTankAngle(game->t1), found);
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
                printf("%0.1f        %0.1f        nr:%d\n", getBulletPositionX(game->bullets[i]), getBulletPositionY(game->bullets[i]),i);
            }
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
    xOLD = getTankPositionX(game->t1);
    yOLD = getTankPositionY(game->t1);
    angleOLD = getTankAngleOLD(game->t1);
    
    updateTankPositionX(game->t1, getTankDx(game->t1));
    updateTankPositionY(game->t1, getTankDy(game->t1));
    setTankAngleOLD(game->t1, getTankAngle(game->t1));
    
    xNEW = getTankPositionX(game->t1);
    yNEW = getTankPositionY(game->t1);
    angleNEW = getTankAngle(game->t1);
    
    
    for (int i = 0; i < MAX_BULLETS; i++) if (game->bullets[i])
    {
        updateBulletPositionX(game->bullets[i], getBulletDx(game->bullets[i]));
        updateBulletPositionY(game->bullets[i], getBulletDy(game->bullets[i]));
    }
    
    if (xOLD != xNEW || yOLD != yNEW || angleOLD != angleNEW) {
        
        sprintf((char *)p->data, "%f %f %f\n", xNEW, yNEW, angleNEW);
        p->address.host = srvadd.host;    /* Set the destination host */
        p->address.port = srvadd.port;    /* And destination port */
        p->len = strlen((char *)p->data) + 1;
        SDLNet_UDP_Send(sd, -1, p);
    }
    
}

void doRender(SDL_Renderer *renderer, GameState *game, int *pnrOfConnections, int *pwhich)
{
    float xCORD, yCORD, dAngle;
    int c;
    if (SDLNet_UDP_Recv(sd, p2)) {
        sscanf((char *)p2->data, "%f %f %d %f %d\n", &xCORD, &yCORD, &c, &dAngle, pnrOfConnections);
        if (c == 1) {
            setEnemyX(game->t2, xCORD);
            setEnemyY(game->t2, yCORD);
            setEnemyAngle(game->t2, dAngle);
        }
        if (c == 2) {
            
            setEnemyX(game->t3, xCORD);
            setEnemyY(game->t3, yCORD);
            setEnemyAngle(game->t3, dAngle);
        }
        if (c == 3) {
            
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
    SDL_Rect secondDest = { getTankPositionX(game->t2), getTankPositionY(game->t2) ,48,48 };
    SDL_Rect thirdDest = { getTankPositionX(game->t3), getTankPositionY(game->t3) ,48,48 };
    SDL_Rect fourthDest = { getTankPositionX(game->t4), getTankPositionY(game->t4) ,48,48 };
    
    if (*pwhich==1) {
        SDL_RenderCopyEx(renderer, game->bild, NULL, &rect, getTankAngle(game->t1) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild3, NULL, &secondDest, getTankAngle(game->t2) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild2, NULL, &thirdDest, getTankAngle(game->t3) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild4, NULL, &fourthDest, getTankAngle(game->t4) + 90, NULL, 0);
    }
    if (*pwhich==2) {
        
        SDL_RenderCopyEx(renderer, game->bild2, NULL, &rect, getTankAngle(game->t1) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild, NULL, &secondDest, getTankAngle(game->t2) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild4, NULL, &thirdDest, getTankAngle(game->t3) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild3, NULL, &fourthDest, getTankAngle(game->t4) + 90, NULL, 0);
    }
    if (*pwhich==3) {
        
        SDL_RenderCopyEx(renderer, game->bild3, NULL, &rect, getTankAngle(game->t1) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild4, NULL, &secondDest, getTankAngle(game->t2) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild, NULL, &thirdDest, getTankAngle(game->t3) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild2, NULL, &fourthDest, getTankAngle(game->t4) + 90, NULL, 0);
    }
    if (*pwhich==4) {
        
        SDL_RenderCopyEx(renderer, game->bild4, NULL, &rect, getTankAngle(game->t1) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild2, NULL, &secondDest, getTankAngle(game->t2) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild3, NULL, &thirdDest, getTankAngle(game->t3) + 90, NULL, 0);
        SDL_RenderCopyEx(renderer, game->bild, NULL, &fourthDest, getTankAngle(game->t4) + 90, NULL, 0);
    }
    if (*pnrOfConnections<4) {
        
        game->font = TTF_OpenFont("/Library/fonts/chalkduster.ttf", 15);
        if (!game->font) {
            printf("Couldnt load font!!\n");
            SDL_Quit();
            exit(1);
        }
        
        SDL_Color Color = {0,0,0};
        SDL_Surface *tmp;
        tmp= TTF_RenderText_Blended(game->font, "Waiting for players...", Color);
        game->waitFlag = SDL_CreateTextureFromSurface(game->renderer, tmp);
        
        SDL_Rect textRect = {320-tmp->w/2, 210, tmp->w, tmp->h};
        int quit = 0;
        
        SDL_RenderCopy(renderer, game->waitFlag, NULL, &textRect);
        
        if (*pnrOfConnections ==4) {
            SDL_FreeSurface(tmp);
            SDL_DestroyTexture(game->waitFlag);
            quit=1;
        }
    }
    
    
    
    
    
    //We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(renderer);
}


