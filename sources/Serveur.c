#include "../headers/Serveur.h"

#define PORT 8000

void receipt(){
    int sock_S;
    char message[128];
    struct sockaddr_in sa_S, sa_C;
    unsigned int taille_sa_S;

    /* Création du socket */
    sock_S = socket(PF_INET, SOCK_DGRAM, 0);
    perror("socket");

    /* @IP et num port serveur */
    bzero( (char*) &sa_S, sizeof(struct sockaddr_in) );
    sa_S.sin_family = PF_INET;
    sa_S.sin_port = htons(PORT);
    sa_S.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Attachement */
    bind(sock_S, (struct sockaddr*) &sa_S, 
        sizeof(struct sockaddr_in));
    perror("bind");

    /* Réception du message */
    taille_sa_S = sizeof(struct sockaddr_in);

    while (1)
    {
        /* Réception du message */
        recvfrom(sock_S, message, 128 * sizeof(char), 0,
                (struct sockaddr*) &sa_C, &taille_sa_S);
        perror("recvfrom : ");

        /* Service : afficahge */
        printf("%s \n", message);

        /* Re-emission datagramme vers client*/
        sendto(sock_S, "Bonjour", 128 * sizeof(char), 0,
                (struct sockaddr*) &sa_C, taille_sa_S);
        perror("sendto : ");
    }

    /* Fin */
    close(sock_S);
    perror("close");
    
    exit(EXIT_SUCCESS);
}