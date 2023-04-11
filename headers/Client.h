#ifndef _CLIENT_H
#define _CLIENT_H


#include "Packet.h"
#include "Appareil.h"
#include "Serveur.h"

void sendData(packet* buf, Appareil* a);
int startSession(Appareil* a, Serveur* s, char* IP[]);

#endif