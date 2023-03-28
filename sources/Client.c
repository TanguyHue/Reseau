#include "../headers/Client.h"
#include "../headers/Packet.h"
#include "../headers/Appareil.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void client(packet* buf, Appareil* a)
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
    sa_S.sin_addr.s_addr = inet_addr(getIPSuivant(a));
    sa_S.sin_port = htons(getUDPport(a));

    if(strcmp(getAdressDest(buf),getIP(a)) == 0)
    {
        if(checksum(buf) == EXIT_FAILURE){
            printf("Erreur checksum\n");
            return;
        }
        printf("Reçu\n");
        setAdressDest(buf, getIPSuivant(a));
        setAdressEmetteur(buf, getIP(a));
    }
    else{
        printf(getAdressDest(buf));
        printf("\n");
    }


    taille_sa_S = sizeof(struct sockaddr);
    sendto(sock_C, buf, sizeof(packet*), 0,(struct sockaddr*) &sa_S, taille_sa_S);
    perror("sendto\n");

    /*fin*/
    close(sock_C);
    perror("close\n");
}