#define _CRT_SECURE_NO_WARNINGS
#include "network.h"

int setupConnection(UDPsocket *sd, IPaddress *srvadd, UDPpacket *p, UDPpacket *p2, int *pnrOfConnections) {
	char ipAddress[25];
	int port, connectionEstablished = 0, timeout = 0, a ,b;
	//get ip and port to server
	printf("Server IP: ");
	scanf("%s", ipAddress);
	printf("Server Port: ");
	scanf("%d", &port);

	/* Resolve server name  */
	if (SDLNet_ResolveHost(srvadd, ipAddress, port) == -1)
	{
		fprintf(stderr, "Couldn't resolve host(%s:%d)\n", ipAddress, port);
		return 0;
	}
	else {
		printf("Connecting to %s:%d\n", ipAddress, port);
	}
	//threewayhandshake
	sprintf((char *)p->data, "%d %d\n", 5, 5);
	p->address.host = srvadd->host;	/* Set the destination host */
	p->address.port = srvadd->port;	/* And destination port */
	p->len = strlen((char *)p->data) + 1;
	SDLNet_UDP_Send(*sd, -1, p);

	while (!connectionEstablished)
	{
		if (SDLNet_UDP_Recv(*sd, p)) {
			sscanf((char *)p->data, "%d\n", pnrOfConnections);
			if (*pnrOfConnections == 5) {
				printf("Server is Full! Please try again later!\n");
				return 0;
			}
			else {
				printf("NEW CONNECTION! THERE IS %d ACTIVE CONNECTIONS\n", *pnrOfConnections);
				connectionEstablished = !connectionEstablished;
				printf("Connected\n");
				return 1;
			}
		}
		else
		{
			//printf("waiting\n");
			if (timeout > 100000) {
				printf("Failed to connect to server, try again\n");
				return 0;
			}
		}
		timeout++;
	}
}
