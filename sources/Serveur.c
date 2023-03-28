#include "../headers/Serveur.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

struct Serveur
{
    int udp_socket;
    struct sockaddr_in sa_Serv;
    struct sockaddr_in sa_Client;
    socklen_t taille_sa;
};

/*
    * Initialisation du serveur
    * @param UDP_port_dest : port d'écoute du serveur
    * @return Serveur* : pointeur sur le serveur
*/
Serveur* initServ(int UDP_port_dest)
{
    Serveur* s = (Serveur*)malloc(sizeof(Serveur));
    s->udp_socket = 0;

    s->udp_socket = socket(PF_INET, SOCK_DGRAM, 0);
    //perror("Création socket !\n");

    bzero((char*) &(s->sa_Serv), sizeof(struct sockaddr));
    s->sa_Serv.sin_family = PF_INET;
    s->sa_Serv.sin_addr.s_addr = htonl(INADDR_ANY);
    s->sa_Serv.sin_port = htons(UDP_port_dest);
    bind(s->udp_socket, (struct sockaddr *) &(s->sa_Serv), sizeof(struct sockaddr));
    //perror("Bind !\n");

    s->taille_sa = sizeof(struct sockaddr);

    return s;
}

/*
    * Attend la réception d'un paquet
    * @param s : pointeur sur le serveur
    * @param buf : pointeur sur le paquet
*/
void receipt(Serveur* s, packet* buf){
    recvfrom(s->udp_socket, buf, sizeof(packet*), 0, (struct sockaddr *) &s->sa_Client, &s->taille_sa);
    //perror("Recvfrom !\n");
}

/*
    * Ferme le serveur
    * @param s : pointeur sur le serveur
*/
void closeServ(Serveur* s){
    close(s->udp_socket);
    //perror("Close !\n");
}

/*
    * Libère la mémoire du serveur
    * @param s : pointeur sur le serveur
*/
void deleteServ(Serveur* s){
    free(s);
}