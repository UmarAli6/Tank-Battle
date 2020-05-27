#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_net.h>
#include <SDL_image.h>

int main(int argc, char **argv)
{
    UDPsocket sd;       /* Socket descriptor */
    UDPpacket *pRecive;       /* Pointer to packet memory */
    UDPpacket *pSent;
    Uint32 IPclient1 = 0;
    Uint32 IPclient2 = 0;
    Uint32 IPclient3 = 0;
    Uint32 IPclient4 = 0;
    Uint32 portClient1 = 0;
    Uint32 portClient2 = 0;
    Uint32 portClient3 = 0;
    Uint32 portClient4 = 0;
    int quit, c, port;
    int nrOfConnections = 0;
    float a, b, d;
    
    printf("Enter port to host server on: ");
    scanf("%d", &port);
    
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    /* Open a socket */
    if (!(sd = SDLNet_UDP_Open(port))) //9565 för online
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    /* Make space for the packet */
    if (!((pSent = SDLNet_AllocPacket(512)) && (pRecive = SDLNet_AllocPacket(512))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    /* Main loop */
    quit = 0;
    printf("Server is running!\n");
    while (!quit)
    {
        
        /* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
        if (SDLNet_UDP_Recv(sd, pRecive))
        {
            
            if (IPclient1 == 0 && portClient1 == 0) {
                printf("\tClient 1 CONNECTED\n");
                printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
                IPclient1 = pRecive->address.host;
                portClient1 = pRecive->address.port;
                nrOfConnections++;
                printf("\tNumber of clients ATM: %d\n", nrOfConnections);
                sprintf((char *)pSent->data, "%d\n", nrOfConnections);
                pSent->address.host = IPclient1;
                pSent->address.port = portClient1;
                pSent->len = strlen((char *)pSent->data) + 1;
                SDLNet_UDP_Send(sd, -1, pSent);
                
            }
            else if (pRecive->address.port != portClient1  && IPclient2 == 0) {
                printf("\tClient 2 CONNECTED\n");
                printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
                IPclient2 = pRecive->address.host;
                portClient2 = pRecive->address.port;
                nrOfConnections++;
                printf("\tNumber of clients ATM: %d\n", nrOfConnections);
                sprintf((char *)pSent->data, "%d\n", nrOfConnections);
                pSent->address.host = IPclient2;
                pSent->address.port = portClient2;
                pSent->len = strlen((char *)pSent->data) + 1;
                SDLNet_UDP_Send(sd, -1, pSent);
            }
            else if (pRecive->address.port != portClient1 && pRecive->address.port != portClient2 && IPclient3 == 0) {
                printf("\tClient 3 CONNECTED\n");
                printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
                IPclient3 = pRecive->address.host;
                portClient3 = pRecive->address.port;
                nrOfConnections++;
                pSent->address.host = IPclient3;
                pSent->address.port = portClient3;
                printf("\tNumber of clients ATM: %d\n", nrOfConnections);
                sprintf((char *)pSent->data, "%d\n", nrOfConnections);
                pSent->len = strlen((char *)pSent->data) + 1;
                SDLNet_UDP_Send(sd, -1, pSent);
            }
            else if (pRecive->address.port != portClient1 && pRecive->address.port != portClient2 && pRecive->address.port != portClient3 &&IPclient4 == 0) {
                printf("\tClient 4 CONNECTED\n");
                printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
                IPclient4 = pRecive->address.host;
                portClient4 = pRecive->address.port;
                nrOfConnections++;
                pSent->address.host = IPclient4;
                pSent->address.port = portClient4;
                printf("\tNumber of clients ATM: %d\n", nrOfConnections);
                sprintf((char *)pSent->data, "%d\n", nrOfConnections);
                pSent->len = strlen((char *)pSent->data) + 1;
                SDLNet_UDP_Send(sd, -1, pSent);
            }
            else if (pRecive->address.port != portClient1 && pRecive->address.port != portClient2 && pRecive->address.port != portClient3 && pRecive->address.port != portClient4) { //skicka att lobby full
                nrOfConnections = 5;
                printf("\NEW CLIENT FAILED TO CONNECT: SERVER FULL\n");
                printf("\tAddress: %x %x\n\n", pRecive->address.host, pRecive->address.port);
                pSent->address.host = pRecive->address.host;
                pSent->address.port = pRecive->address.port;
                sprintf((char *)pSent->data, "%d\n", nrOfConnections);
                pSent->len = strlen((char *)pSent->data) + 1;
                SDLNet_UDP_Send(sd, -1, pSent);
            }
            else {
                printf("UDP Packet recived!:\n");
                if (pRecive->address.port == portClient1) {
                    if (strcmp((char *)pRecive->data, "Reset\n") == 0){
                        IPclient1 = 0;
                        portClient1 = 0;
                        nrOfConnections = nrOfConnections - 1;
                    }
                    else
                    {
                        //SCAN THE RECIVED PACKETS CONTENT TO A&B
                        sscanf((char *)pRecive->data, "%f %f %f\n", &a, &b, &d);
                        //PRINT PACKET CONTENT TO TERMINAL FOR DEBUGGING
                        printf("CLIENT 1 PACKET CONTENT: %0.1f %0.1f %0.1f\n", a, b, d);
                        for (int i = 1; i < 5; i++)
                        {
                            switch (i)
                            {
                                case 2:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient2;
                                    pSent->address.port = portClient2;
                                    c = 1;
                                    break;
                                case 3:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient3;
                                    pSent->address.port = portClient3;
                                    c = 2;
                                    break;
                                case 4:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient4;
                                    pSent->address.port = portClient4;
                                    c = 3;
                                    break;
                                default:
                                    break;
                            }
                            
                            if (i != 1)
                            {
                                if (a == 5 && b == 5 && d == 5) {
                                    //Skicka ett skott
                                    a = b = d = 6;
                                    sprintf((char *)pSent->data, "%f %f %d %f %d\n", a, b, c, d, nrOfConnections);
                                    // DETERMINE LENGTH OF PACKET
                                    pSent->len = strlen((char *)pSent->data) + 1;
                                    //SEND PACKET
                                    SDLNet_UDP_Send(sd, -1, pSent);
                                }
                                else {
                                    //Skicka vanlig data
                                    //PACK PSENT WITH A&B
                                    sprintf((char *)pSent->data, "%f %f %d %f %d\n", a, b, c, d, nrOfConnections);
                                    // DETERMINE LENGTH OF PACKET
                                    pSent->len = strlen((char *)pSent->data) + 1;
                                    //SEND PACKET
                                    SDLNet_UDP_Send(sd, -1, pSent);
                                }
                            }
                        }
                        printf("\n");
                        
                    }
                }
                else if (pRecive->address.port == portClient2) {
                    if (strcmp((char *)pRecive->data, "Reset\n") == 0){
                        IPclient2 = 0;
                        portClient2 = 0;
                        nrOfConnections = nrOfConnections - 1;
                    }
                    else
                    {
                        //SCAN THE RECIVED PACKETS CONTENT TO A&B
                        sscanf((char *)pRecive->data, "%f %f %f\n", &a, &b, &d);
                        //PRINT PACKET CONTENT TO TERMINAL FOR DEBUGGING
                        printf("CLIENT 2 PACKET CONTENT: %0.1f %0.1f %0.1f\n", a, b, d);
                        for (int i = 1; i < 5; i++)
                        {
                            switch (i)
                            {
                                case 1:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient1;
                                    pSent->address.port = portClient1;
                                    c = 2;
                                    break;
                                case 3:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient3;
                                    pSent->address.port = portClient3;
                                    c = 3;
                                    break;
                                case 4:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient4;
                                    pSent->address.port = portClient4;
                                    c = 1;
                                    break;
                                default:
                                    break;
                            }
                            
                            if (i != 2)
                            {
                                if (a == 5 && b == 5 && d == 5) {
                                    //Skicka ett skott
                                    a = b = d = 6;
                                    sprintf((char *)pSent->data, "%f %f %d %f %d\n", a, b, c, d, nrOfConnections);
                                    // DETERMINE LENGTH OF PACKET
                                    pSent->len = strlen((char *)pSent->data) + 1;
                                    //SEND PACKET
                                    SDLNet_UDP_Send(sd, -1, pSent);
                                }
                                else {
                                    //PACK PSENT WITH A&B
                                    sprintf((char *)pSent->data, "%f %f %d %f %d\n", a, b, c, d, nrOfConnections);
                                    // DETERMINE LENGTH OF PACKET
                                    pSent->len = strlen((char *)pSent->data) + 1;
                                    //SEND PACKET
                                    SDLNet_UDP_Send(sd, -1, pSent);
                                }
                            }
                        }
                        printf("\n");
                    }
                }
                else if (pRecive->address.port == portClient3) {
                    if (strcmp((char *)pRecive->data, "Reset\n") == 0){
                        IPclient3 = 0;
                        portClient3 = 0;
                        nrOfConnections = nrOfConnections - 1;
                    }
                    else
                    {
                        //SCAN THE RECIVED PACKETS CONTENT TO A&B
                        sscanf((char *)pRecive->data, "%f %f %f\n", &a, &b, &d);
                        //PRINT PACKET CONTENT TO TERMINAL FOR DEBUGGING
                        printf("CLIENT 3 PACKET CONTENT: %0.1f %0.1f %0.1f\n", a, b, d);
                        for (int i = 1; i < 5; i++)
                        {
                            switch (i)
                            {
                                case 1:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient1;
                                    pSent->address.port = portClient1;
                                    c = 1;
                                    break;
                                case 2:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient2;
                                    pSent->address.port = portClient2;
                                    c = 3;
                                    break;
                                case 4:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient4;
                                    pSent->address.port = portClient4;
                                    c = 2;
                                    break;
                                default:
                                    break;
                            }
                            
                            if (i != 3)
                            {
                                if (a == 5 && b == 5 && d == 5) {
                                    //Skicka ett skott
                                    a = b = d = 6;
                                    sprintf((char *)pSent->data, "%f %f %d %f %d\n", a, b, c, d, nrOfConnections);
                                    // DETERMINE LENGTH OF PACKET
                                    pSent->len = strlen((char *)pSent->data) + 1;
                                    //SEND PACKET
                                    SDLNet_UDP_Send(sd, -1, pSent);
                                }
                                else {
                                    //PACK PSENT WITH A&B
                                    sprintf((char *)pSent->data, "%f %f %d %f %d\n", a, b, c, d, nrOfConnections);
                                    // DETERMINE LENGTH OF PACKET
                                    pSent->len = strlen((char *)pSent->data) + 1;
                                    //SEND PACKET
                                    SDLNet_UDP_Send(sd, -1, pSent);
                                }
                            }
                        }
                        printf("\n");
                        
                    }
                }
                else if (pRecive->address.port == portClient4) {
                    if (strcmp((char *)pRecive->data, "Reset\n") == 0){
                        IPclient4 = 0;
                        portClient4 = 0;
                        nrOfConnections = nrOfConnections - 1;
                    }
                    else
                    {
                        //SCAN THE RECIVED PACKETS CONTENT TO A&B
                        sscanf((char *)pRecive->data, "%f %f %f\n", &a, &b, &d);
                        //PRINT PACKET CONTENT TO TERMINAL FOR DEBUGGING
                        printf("CLIENT 4 PACKET CONTENT: %0.1f %0.1f %0.1f\n", a, b, d);
                        for (int i = 1; i < 5; i++)
                        {
                            switch (i)
                            {
                                case 1:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient1;
                                    pSent->address.port = portClient1;
                                    c = 3;
                                    break;
                                case 2:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient2;
                                    pSent->address.port = portClient2;
                                    c = 2;
                                    break;
                                case 3:
                                    //SET DESTINATION HOST
                                    pSent->address.host = IPclient3;
                                    pSent->address.port = portClient3;
                                    c = 1;
                                    break;
                                default:
                                    break;
                            }
                            
                            if (i != 4)
                            {
                                if (a == 5 && b == 5 && d == 5) {
                                    //Skicka ett skott
                                    a = b = d = 6;
                                    sprintf((char *)pSent->data, "%f %f %d %f %d\n", a, b, c, d, nrOfConnections);
                                    // DETERMINE LENGTH OF PACKET
                                    pSent->len = strlen((char *)pSent->data) + 1;
                                    //SEND PACKET
                                    SDLNet_UDP_Send(sd, -1, pSent);
                                }
                                else {
                                    //PACK PSENT WITH A&B
                                    sprintf((char *)pSent->data, "%f %f %d %f %d\n", a, b, c, d, nrOfConnections);
                                    // DETERMINE LENGTH OF PACKET
                                    pSent->len = strlen((char *)pSent->data) + 1;
                                    //SEND PACKET
                                    SDLNet_UDP_Send(sd, -1, pSent);
                                }
                            }
                        }
                        
                        printf("\n");
                    }
                    
                }
                    
            }
        }
        
    }
    /* Clean and exit */
    SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pRecive);
    SDLNet_Quit();
    return EXIT_SUCCESS;
    
}
