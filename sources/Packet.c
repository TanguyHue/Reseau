#include "../headers/Packet.h"
#include "../headers/Appareil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PACKET_SIZE 1024

typedef struct packet packet;
struct packet {
    int size;
    char adress_emetteur[16];
    char adress_destinataire[16];
    char data[PACKET_SIZE];
    int checksum;
};

packet* createPacket(char* data, Appareil* a) {
    packet* p = (packet*) malloc(sizeof(packet));
    p->size = strlen(data);
    strcpy(p->adress_emetteur, getIP(a));
    strcpy(p->adress_destinataire, getIPSuivant(a));
    memcpy(p->data, data, p->size);
    p->checksum = sum(p->data, p->size);
    return p;
}

int sum(char* data, int len) {
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += data[i];
    }
    return sum;
}

int checksum (packet* p) {
    if (sum(p->data, p->size) == p->checksum) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

packet* tokenPacket(Appareil* a) {
    return createPacket("", a);
}

int isTokenPacket(packet* p) {
    return p->size == 0;
}

int getSize(packet* p) {
    return p->size;
}

char* getData(packet* p) {
    return p->data;
}

void setData(packet* p, char* data) {
    strcpy(p->data, data);
    p->size = strlen(data);
    p->checksum = sum(p->data, p->size);
}

void setAdressDest(packet* p, char* adress_dest){
    strcpy(p->adress_destinataire, adress_dest);
}

void setAdressEmetteur(packet* p, char* adress_emetteur){
    strcpy(p->adress_emetteur, adress_emetteur);
}

char* getAdressDest(packet* p){
    return p->adress_destinataire;
}

char* getAdressEmetteur(packet* p){
    return p->adress_emetteur;
}

void deletePacket(packet* p) {
    free(p);
}