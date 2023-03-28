#include "../headers/Serveur.h"

void init(Serveur * s, int UDP_port_dest)
{
    s->udp_socket = socket(PF_INET, SOCK_DGRAM, 0);
    perror("Création socket !\n");

    bzero((char*) &(s->sa_Serv), sizeof(struct sockaddr));
    s->sa_Serv.sin_family = PF_INET;
    s->sa_Serv.sin_addr.s_addr = htonl(INADDR_ANY);
    s->sa_Serv.sin_port = htons(UDP_port_dest);
    bind(s->udp_socket, (struct sockaddr *) &(s->sa_Serv), sizeof(struct sockaddr));
    perror("Bind !\n");

    s->taille_sa = sizeof(struct sockaddr);
}

void receipt(Serveur* s, packet* buf){
    recvfrom(s->udp_socket, buf, sizeof(packet*), 0, (struct sockaddr *) &(s->sa_Client), &(s->taille_sa));
    perror("Recvfrom !\n");
}

void stop(Serveur* s){
    close(s->udp_socket);
    perror("Close !\n");
}