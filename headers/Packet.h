#ifndef _PACKET_H
#define _PACKET_H

#include "Appareil.h"

typedef struct packet packet;
struct packet {
    char* adress_emetteur;
    char* adress_destinataire;
    char* data;
    int size;
    int checksum;
};

packet* createPacket(char* data, Appareil* a);
packet* tokenPacket(Appareil* a);
int isTokenPacket(packet* p);
int getSize(packet* p);
char* getData(packet* p);
void setData(packet* p, char* data);
void setAdressDest(packet* p, char* adress_dest);
void setAdressEmetteur(packet* p, char* adress_emetteur);
char* getAdressDest(packet* p);
char* getAdressEmetteur(packet* p);
int sum(char* data, int len);
int checksum (packet* p);
void deletePacket(packet* p);
int checkIP(Appareil* a, packet* p);

#endif