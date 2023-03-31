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
int receipt(Serveur* s, packet* buf){
    char buffer[BUF_SIZE];
    recvfrom(s->udp_socket, buffer, BUF_SIZE, 0, NULL, NULL);
    printf("Buffer : %s\n", buffer);

    printf("!==Paquet reçu==!\n");
    char temp[10];
    strncpy(temp, buffer, 9);
    temp[9] = '\0';
    int taille_ip_emetteur = ntohl(atoi(temp));
    memcpy(buf->adress_emetteur, buffer+9, taille_ip_emetteur);
    printf("IP emetteur : %s\n", buf->adress_emetteur);

    strncpy(temp, buffer+9+taille_ip_emetteur, 8);
    temp[9] = '\0';

    int taille_ip_dest = ntohl(atoi(temp));

    memcpy(buf->adress_destinataire, buffer+9+taille_ip_emetteur+9, taille_ip_dest);
    printf("IP destinataire : %s\n", buf->adress_destinataire);
    
    strncpy(temp, buffer+9+taille_ip_emetteur+9+taille_ip_dest, 9);
    temp[9] = '\0';

    int taille_data = ntohl(atoi(temp));

    memcpy(buf->data, buffer+9+taille_ip_emetteur+9+taille_ip_dest+8, taille_data);
    printf("Data : %s\n", buf->data);
    memcpy(&(buf->size), &taille_data, sizeof(int));
    printf("Taille data : %d\n", buf->size);

    strncpy(temp, buffer+8+taille_ip_emetteur+9+taille_ip_dest+9+taille_data, 9);
    temp[9] = '\0';

    memcpy(&(buf->checksum), temp, 9);
    printf("Checksum : %d\n", buf->checksum);

    printf("!==Fin paquet==!\n");

    return EXIT_SUCCESS;
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