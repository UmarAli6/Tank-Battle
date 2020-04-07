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
	int quit, a, b;

	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Open a socket */
	if (!(sd = SDLNet_UDP_Open(2000)))
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
	printf("Server");
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pRecive))
		{
			if (IPclient1 == 0 && portClient1 == 0) {
				printf("Client 1\n");
				IPclient1 = pRecive->address.host;
				portClient1 = pRecive->address.port;
			}
			else if (pRecive->address.port != portClient1  && IPclient2 == 0) {
				printf("Client 2\n");
				IPclient2 = pRecive->address.host;
				portClient2 = pRecive->address.port;
			}
			else if (pRecive->address.port != portClient2 && pRecive->address.port != portClient1 && IPclient3 == 0) {
				printf("Client 3\n");
				IPclient3 = pRecive->address.host;
				portClient3 = pRecive->address.port;
			}
			else if (pRecive->address.port != portClient2 && pRecive->address.port != portClient1 && pRecive->address.port != portClient3 && IPclient4 == 0) {
				printf("Client 4\n");
				IPclient4 = pRecive->address.host;
				portClient4 = pRecive->address.port;
			}
			else {
				if (pRecive->address.port == portClient1) {
					if (IPclient2 != 0 && IPclient3 != 0 && IPclient4 != 0) {
						for (int i = 1; i < 5; i++)
						{
							switch (i)
							{
							default:
								break;
							case 2:
								pSent->address.host = IPclient2;
								pSent->address.port = portClient2;
								break;
							case 3:
								pSent->address.host = IPclient3;
								pSent->address.port = portClient3;
								break;
							case 4:
								pSent->address.host = IPclient4;
								pSent->address.port = portClient4;
								break;
							}
							sscanf((char *)pRecive->data, "%d %d\n", &a, &b);
							printf("%d %d\n", a, b);
							sprintf((char *)pSent->data, "%d %d\n", a, b);
							pSent->len = strlen((char *)pSent->data) + 1;
							SDLNet_UDP_Send(sd, -1, pSent);
						}
					}
				}
				else if (pRecive->address.port == portClient2) {
					if (IPclient1 != 0 && IPclient3 != 0 && IPclient4 != 0) {
						for (int i = 1; i < 5; i++)
						{
							switch (i)
							{
							default:
								break;
							case 1:
								pSent->address.host = IPclient1;
								pSent->address.port = portClient1;
								break;
							case 3:
								pSent->address.host = IPclient3;
								pSent->address.port = portClient3;
								break;
							case 4:
								pSent->address.host = IPclient4;
								pSent->address.port = portClient4;
								break;
							}
							sscanf((char *)pRecive->data, "%d %d\n", &a, &b);
							printf("%d %d\n", a, b);
							sprintf((char *)pSent->data, "%d %d\n", a, b);
							pSent->len = strlen((char *)pSent->data) + 1;
							SDLNet_UDP_Send(sd, -1, pSent);
						}
					}
				}
				else if (pRecive->address.port == portClient3) {
					if (IPclient1 != 0 && IPclient2 != 0 && IPclient4 != 0) {
						for (int i = 1; i < 5; i++)
						{
							switch (i)
							{
							default:
								break;
							case 1:
								pSent->address.host = IPclient1;
								pSent->address.port = portClient1;
								break;
							case 2:
								pSent->address.host = IPclient2;
								pSent->address.port = portClient2;
								break;
							case 4:
								pSent->address.host = IPclient4;
								pSent->address.port = portClient4;
								break;
							}
							sscanf((char *)pRecive->data, "%d %d\n", &a, &b);
							printf("%d %d\n", a, b);
							sprintf((char *)pSent->data, "%d %d\n", a, b);
							pSent->len = strlen((char *)pSent->data) + 1;
							SDLNet_UDP_Send(sd, -1, pSent);
						}
					}
				}
				else if (pRecive->address.port == portClient4) {
					if (IPclient1 != 0 && IPclient2 != 0 && IPclient3 != 0) {
						for (int i = 1; i < 5; i++)
						{
							switch (i)
							{
							default:
								break;
							case 1:
								pSent->address.host = IPclient1;
								pSent->address.port = portClient1;
								break;
							case 2:
								pSent->address.host = IPclient2;
								pSent->address.port = portClient2;
								break;
							case 3:
								pSent->address.host = IPclient3;
								pSent->address.port = portClient3;
								break;
							}
							sscanf((char *)pRecive->data, "%d %d\n", &a, &b);
							printf("%d %d\n", a, b);
							sprintf((char *)pSent->data, "%d %d\n", a, b);
							pSent->len = strlen((char *)pSent->data) + 1;
							SDLNet_UDP_Send(sd, -1, pSent);
						}
					}
				}

			}
			/* Quit if packet contains "quit" */
			if (strcmp((char *)pSent->data, "quit") == 0)
				quit = 1;
		}
	}

	/* Clean and exit */
	SDLNet_FreePacket(pSent);
	SDLNet_FreePacket(pRecive);
	SDLNet_Quit();
	return EXIT_SUCCESS;
}
