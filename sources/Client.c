#include "../headers/Client.h"

#define PORT 8000
#define IP "172.19.66.68"

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define UDP_port_S 8002
#define IP_addr_S "172.19.66.62"

void client(char* buf)
{
    int sock_C; 
    struct sockaddr_in sa_S;
    unsigned int taille_sa_S;

    /* Création socket client */
    sock_C = socket(PF_INET, SOCK_DGRAM, 0); 
    perror("socket\n");

    /* @IP et num port Serveur */
    bzero((char*) &sa_S, sizeof( struct sockaddr));
    sa_S.sin_family = AF_INET;
    sa_S.sin_addr.s_addr = inet_addr(IP_addr_S);
    sa_S.sin_port = htons(UDP_port_S);


    taille_sa_S = sizeof(struct sockaddr);
    sendto(sock_C, buf, 128 * sizeof(char), 0,(struct sockaddr*) &sa_S, taille_sa_S);
    perror("sendto\n");

    /*fin*/
    close( sock_C);
    perror("close\n");
}