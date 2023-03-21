#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define PORT 8000
#define IP "127.0.0.1"

int main(int argc, char* argv[]){
    int sock_C;
    struct sockaddr_in sa_S;
    unsigned int taille_sa_S;
    char message[128];

    /* Création du socket */
    sock_C = socket(PF_INET, SOCK_DGRAM, 0);
    perror("socket");

    /* @IP et num port serveur */
    bzero( (char*) &sa_S, sizeof(struct sockaddr_in) );
    sa_S.sin_family = AF_INET;
    sa_S.sin_port = htons(PORT);
    sa_S.sin_addr.s_addr = inet_addr(IP);

    /* Envoi du message */
    taille_sa_S = sizeof(struct sockaddr_in);

    sendto(sock_C, "Bonjour", 128 * sizeof(char), 0,
            (struct sockaddr*) &sa_S, taille_sa_S);
    perror("sendto");

    /* Réception du message */
    recvfrom(sock_C, message, 128 * sizeof(char), 0,
            (struct sockaddr*) &sa_S, &taille_sa_S);
    perror("recvfrom");

    printf("%s \n", message);

    /* Fermeture du socket */
    close(sock_C);
    perror("close");

    exit(EXIT_SUCCESS);
}