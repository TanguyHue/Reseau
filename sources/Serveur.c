#include "../headers/Serveur.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 256

struct Serveur
{
    int udp_socket;
    struct sockaddr_in sa_Serv;
    struct sockaddr_in sa_Client;
    socklen_t taille_sa;
};

/*
    * Initialisation du serveur
    * @param UDP_port_dest port d'écoute du serveur
    * @return Serveur* : pointeur sur le serveur
*/
Serveur* initServ(int UDP_port_dest)
{
    Serveur* s = (Serveur*)malloc(sizeof(Serveur));
    s->udp_socket = 0;

    s->udp_socket = socket(PF_INET, SOCK_DGRAM, 0);

    bzero((char*) &(s->sa_Serv), sizeof(struct sockaddr));
    s->sa_Serv.sin_family = PF_INET;
    s->sa_Serv.sin_addr.s_addr = htonl(INADDR_ANY);
    s->sa_Serv.sin_port = htons(UDP_port_dest);
    bind(s->udp_socket, (struct sockaddr *) &(s->sa_Serv), sizeof(struct sockaddr));

    s->taille_sa = sizeof(struct sockaddr);

    return s;
}

/*
    * Attend la réception d'un paquet
    * @param s pointeur sur le serveur
    * @param buf pointeur sur le paquet
*/
int receipt(Serveur* s, packet* buf){
    char buffer[BUF_SIZE];
    recvfrom(s->udp_socket, buffer, BUF_SIZE, 0, NULL, NULL);

    char temp[5];
    strncpy(temp, buffer, 4);
    temp[5] = '\0';
    int taille_ip_emetteur = (atoi(temp));
    memset(buf->adress_emetteur, 0, strlen(buf->adress_emetteur));
    memcpy(buf->adress_emetteur, buffer+4, taille_ip_emetteur);

    strncpy(temp, buffer+4+taille_ip_emetteur, 4);
    temp[5] = '\0';

    int taille_ip_dest = (atoi(temp));

    memset(buf->adress_destinataire, 0, strlen(buf->adress_destinataire));
    memcpy(buf->adress_destinataire, buffer+4+taille_ip_emetteur+4, taille_ip_dest);
    
    strncpy(temp, buffer+4+taille_ip_emetteur+4+taille_ip_dest, 4);
    temp[5] = '\0';

    int taille_data = (atoi(temp));

    memset(buf->data, 0, strlen(buf->data));
    memcpy(buf->data, buffer+4+taille_ip_emetteur+4+taille_ip_dest+4, taille_data);

    buf->size = 0;
    memcpy(&(buf->size), &taille_data, sizeof(int));


    strncpy(temp, buffer+4+taille_ip_emetteur+4+taille_ip_dest+4+taille_data, 4);
    temp[5] = '\0';

    buf->checksum = 0;
    memcpy(&(buf->checksum), temp, 4);

    return EXIT_SUCCESS;
}

/*
    * Ferme le serveur
    * @param s pointeur sur le serveur
*/
void closeServ(Serveur* s){
    close(s->udp_socket);
}

/*
    * Libère la mémoire du serveur
    * @param s pointeur sur le serveur
*/
void deleteServ(Serveur* s){
    free(s);
}
