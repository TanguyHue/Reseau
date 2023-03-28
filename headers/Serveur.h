#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "Packet.h"

typedef struct Serveur Serveur;
struct Serveur;

Serveur* initServ(int UDP_port_dest);
void closeServ(Serveur* s);
void deleteServ(Serveur* s);
void receipt(Serveur* s, packet* buf);
void stop(Serveur* s);
