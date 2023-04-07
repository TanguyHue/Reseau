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

#define BUFFER_SIZE 256

char* completeInt(int n);

/*
    * Fonction qui envoie un paquet à l'appareil suivant
    * @param buf paquet à envoyer
    * @param a appareil qui envoie le paquet
*/
void sendData(packet* buf, Appareil* a)
{
    int sock_C; 
    struct sockaddr_in sa_S;
    unsigned int taille_sa_S;

    // Création socket client 
    sock_C = socket(PF_INET, SOCK_DGRAM, 0); 


    // IP et num port Serveur 
    bzero((char*) &sa_S, sizeof( struct sockaddr));
    sa_S.sin_family = AF_INET;
    sa_S.sin_addr.s_addr = inet_addr(getIPSuivant(a));
    sa_S.sin_port = htons(getUDPport(a));

    taille_sa_S = sizeof(struct sockaddr);

    // Conversion de la structure en un tableau de caractères
    char packet_buffer[BUFFER_SIZE];

    int size_ip_emetteur = strlen(buf->adress_emetteur);
    int size_ip_destinataire = strlen(buf->adress_destinataire);

    /*
        Structure de la trame
        - Nombre de caractère de l'@ IP de l'émetteur
        - @ IP de l'émetteur
        - Nombre de caractère de l'@ IP du destinataire
        - @ IP du destinataire
        - Nombre de caractère des données
        - Données
        - CheckSum
    */
    sprintf(packet_buffer, "%s%s%s%s%s%s%s\n", completeInt(size_ip_emetteur), buf->adress_emetteur, completeInt(size_ip_destinataire), buf->adress_destinataire, completeInt(buf->size), buf->data, completeInt(buf->checksum));


    sendto(sock_C, packet_buffer, strlen(packet_buffer), 0,(struct sockaddr*) &sa_S, taille_sa_S);

    close(sock_C);
}

/*
    * Fonction qui remplit les octets d'un entier pour qu'il fasse 4 caractères
    * @param n entier à compléter
    * @return char* entier complété
*/
char* completeInt(int n)
{
    char* res = (char*)malloc(5*sizeof(char));
    sprintf(res, "%d", n);
    int size = strlen(res);
    char* padded_res = (char*)malloc(5*sizeof(char));
    for(int i = 0; i < 4-size; i++)
    {
        strcat(padded_res, "0");
    }
    strcat(padded_res, res);
    free(res); // libérer la mémoire de la chaîne d'origine
    return padded_res;
}