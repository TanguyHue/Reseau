#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "Packet.h"

typedef struct Serveur Serveur;
struct Serveur
{
    int udp_socket;
    struct sockaddr_in sa_Serv, sa_Client;
    unsigned int taille_sa;
};

void init(Serveur* s,  int UDP_port_dest);
void receipt(Serveur* s, packet* buf);
void stop(Serveur* s);
