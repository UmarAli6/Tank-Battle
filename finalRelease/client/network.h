#pragma once

#ifndef network_h
#define network_h

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "game.h"

int setupConnection(UDPsocket *sd, IPaddress *srvadd, UDPpacket *p, UDPpacket *p2, int *pnrOfConnections);

#endif /* network.h */
