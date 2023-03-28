#ifndef _SERVEUR_H
#define _SERVEUR_H

#include "Packet.h"

typedef struct Serveur Serveur;
struct Serveur;

Serveur* initServ(int UDP_port_dest);
void closeServ(Serveur* s);
void deleteServ(Serveur* s);
void receipt(Serveur* s, packet* buf);

#endif
