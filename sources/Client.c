#include "../headers/Client.h"
#include "../headers/Packet.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void client(packet* buf, char* IP_expe, char* IP_dest, int port_dest)
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
    sa_S.sin_addr.s_addr = inet_addr(IP_dest);
    sa_S.sin_port = htons(port_dest);

    if(strcmp(getAdressDest(buf),IP_expe) == 0)
    {
        if(checksum(buf) == EXIT_FAILURE){
            printf("Erreur checksum\n");
            return;
        }
        printf("Reçu\n");
        setAdressDest(buf, IP_dest);
        setAdressEmetteur(buf, IP_expe);
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