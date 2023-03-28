#include "headers/Client.h"
#include "headers/Serveur.h"
#include "headers/Packet.h"

int main(int argc, char* argv[])
{
    // Gestion du destinataire
    int UDP_port_dest = 8000;
    char* IP_expe = (char*)malloc(sizeof(char) * 16);
    char* IP_dest = (char*)malloc(sizeof(char) * 16);
    if(argc == 1){
        printf("Indiquer le numéro du client\n");
        return EXIT_FAILURE;
    }
    else{
        if(argc > 2){
            printf("Trop d'arguments\n");
            return EXIT_FAILURE;
        }
        
        if(strcmp(argv[1], "l") == 0){
            strcpy(IP_expe, "127.0.0.1\0");
            strcpy(IP_dest, "127.0.0.1\0");
            printf("Client local : %s\n", IP_expe);
        }
        else if(strcmp(argv[1], "1") == 0){
            strcpy(IP_expe, "172.19.70.26\0");
            strcpy(IP_dest, "172.19.70.27\0");
            printf("Client 1 : %s\n", IP_expe);
        }
        else if(strcmp(argv[1], "2") == 0){
            strcpy(IP_expe, "172.19.70.27\0");
            strcpy(IP_dest, "172.19.70.28\0");
            printf("Client 1 : %s\n", IP_expe);
        }
        else if(strcmp(argv[1], "3") == 0){
            strcpy(IP_expe, "172.19.70.28\0");
            strcpy(IP_dest, "172.19.70.29\0");
            printf("Client 1 : %s\n", IP_expe);
        }
        else{
            printf("Client inconnu\n");
            return EXIT_FAILURE;
        }
    }

    int nb_boucle = 0;

    // Gestion du serveur
    Serveur s;
    packet* p = createPacket("Salut ! \0", IP_expe, "*\0");   

    setData(p, "Coucou toi...\0");
    if(strcmp(argv[1], "1") == 0 || strcmp(argv[1], "l") == 0){
        client(p, IP_expe, IP_dest, UDP_port_dest);
    }

    while(1)
    {
        receipt(&s, p);
        printf("Nombre de boucle : %d\n", nb_boucle);
        nb_boucle++;
        printf("Message envoyé : %s\n", getData(p));
        client(p, IP_expe, IP_dest, UDP_port_dest);
        sleep(1);
    }

    return EXIT_SUCCESS;
}