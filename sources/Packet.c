#include "../headers/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PACKET_SIZE 1024


/*
    * Initialise un paquet
    * @param data données à envoyer
    * @param a appareil qui envoie le paquet
    * @return packet* : paquet initialisé
*/
packet* createPacket(char* data, Appareil* a) {
    packet* p = (packet*) malloc(sizeof(packet));
    p->size = strlen(data);
    p->adress_emetteur = (char*) malloc(sizeof(char) * 16);
    p->adress_destinataire = (char*) malloc(sizeof(char) * 16);
    p->data = (char*) malloc(sizeof(char) * PACKET_SIZE);
    strcpy(p->adress_emetteur, getIP(a));
    strcpy(p->adress_destinataire, getIPSuivant(a));
    memcpy(p->data, data, p->size);
    p->checksum = sum(p->data, p->size);
    return p;
}

/*
    * Calcul le checksum d'un paquet
    * @param p données du paquet
    * @param len taille des données
    * @return : int checksum du paquet
*/
int sum(char* data, int len) {
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += data[i];
    }
    return sum;
}

/*
    * Fais un test de checksum
    * @param p paquet
    * @return int : 1 si le checksum est bon, 0 sinon
*/
int checksum (packet* p) {
    if (sum(p->data, p->size) == p->checksum) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

/*
    * Initialise un paquet token
    * @return packet* : paquet token initialisé
*/
packet* tokenPacket() {
    Appareil* a = initAppareilParam("NULL", "1111111", "1111111", 1111111);
    return createPacket("", a);
}

packet* resetPacket(Appareil* a){
    Appareil* a2 = initAppareilParam("NULL", getIP(a), "000000", 1111111);
    return createPacket("", a2);
}

/*
    * Vérifie si un paquet est un paquet token
    * @param p paquet
    * @return int : 1 si le paquet est un paquet token, 0 sinon
*/
int isTokenPacket(packet* p) {
    return p->size == 0;
}

/*
    * Récupère la taille d'un paquet
    * @param p paquet
    * @return int : taille du paquet
*/
int getSize(packet* p) {
    return p->size;
}

/*
    * Récupère les données d'un paquet
    * @param p paquet
    * @return char* : données du paquet
*/
char* getData(packet* p) {
    return p->data;
}

/*
    * Modifie les données d'un paquet
    * @param p paquet
    * @param data données à envoyer
*/
void setData(packet* p, char* data) {
    strcpy(p->data, data);
    p->size = strlen(data);
    p->checksum = sum(p->data, p->size);
}

/*
    * Modifie l'adresse du destinataire d'un paquet
    * @param p paquet
    * @param adress_dest adresse du destinataire
*/
void setAdressDest(packet* p, char* adress_dest){
    strcpy(p->adress_destinataire, adress_dest);
}

/*
    * Modifie l'adresse de l'émetteur d'un paquet
    * @param p paquet
    * @param adress_emetteur adresse de l'émetteur
*/
void setAdressEmetteur(packet* p, char* adress_emetteur){
    strcpy(p->adress_emetteur, adress_emetteur);
}

/*
    * Récupère l'adresse du destinataire d'un paquet
    * @param p paquet
    * @return char* : adresse du destinataire
*/
char* getAdressDest(packet* p){
    return p->adress_destinataire;
}

/*
    * Récupère l'adresse de l'émetteur d'un paquet
    * @param p paquet
    * @return char* : adresse de l'émetteur
*/
char* getAdressEmetteur(packet* p){
    return p->adress_emetteur;
}

/*
    * Libère la mémoire allouée pour un paquet
    * @param p paquet à libérer
*/
void deletePacket(packet* p) {
    free(p);
}

/*
    * Vérifie si l'adresse de l'appareil correspond à l'adresse du destinataire du paquet
    * @param a appareil
    * @param paquet
    * @return int : 1 si l'adresse de l'appareil correspond à l'adresse du destinataire du paquet, 0 sinon
*/
int checkIP(Appareil* a, packet* p){
    return(strcmp(getIP(a), getAdressDest(p)) == 0);
}


/*
    * Vérifie si le packet est un token
    * @param p paquet
    * @return int : 1 si le paquet est un token, 0 sinon
*/
int checkToken(packet* p){
    return(strcmp(getAdressEmetteur(p), "1111111") == 0);
}

/*
    * Gère les tokens de réinitialisation
    * @param p paquet
    * @param a Appareil qui reçoit
    * @return int : 0 si le paquet à fait un tour, 1 si ce paquet est un paquet de reset, 2 sinon
*/
int checkReset(packet* p, Appareil* a){
    if(strcmp(getAdressDest(p), "000000") == 0 && strcmp(getAdressEmetteur(p), getIP(a)) == 0){
        return 0;
    } else if(strcmp(getAdressDest(p), "000000") == 0 && strcmp(getAdressEmetteur(p), getIP(a)) != 0){
        return 1;
    } else {
        return 2;
    }
}