#define _XOPEN_SOURCE 700
#include "headers/Client.h"
#include "headers/Serveur.h"
#include "headers/Packet.h"
#include "headers/Appareil.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Ports d'écoute des différents appareils
#define UDP_1 8011
#define UDP_2 8012
#define UDP_3 8013
#define UDP_4 8014

// Adresses IP des appareils
char IP_1[128] = "127.0.0.1\0";
char IP_2[128] = "127.0.0.2\0";
char IP_3[128] = "127.0.0.3\0";
char IP_4[128] = "127.0.0.4\0";



// 172.19.65.111 <- 1
// 172.19.65.112 <- 2
// 172.19.65.114 <- 3
// 172.19.65.115 <- 4

// Saisie des IP des machines, pour les réseaux externes
void lire_IP();

int main(int argc, char* argv[])
{   
    Appareil* machine;
    Serveur* serveur;
    if(argc == 3){
        // Boucle locale
        if(strcmp(argv[2], "-r") == 0) {
            printf("Boucle externe\n");

            lire_IP();
            if(strcmp(argv[1], "1") == 0){
                machine = initAppareil("machine 1\0", IP_1, IP_2, UDP_2);
                serveur = initServ(UDP_1);
            }
            else if(strcmp(argv[1], "2") == 0){
                machine = initAppareil("machine 2\0", IP_1, IP_2, UDP_3);
                serveur = initServ(UDP_2);
            }
            else if(strcmp(argv[1], "3") == 0){
                machine = initAppareil("machine 3\0", IP_1, IP_2, UDP_4);
                serveur = initServ(UDP_3);
            }
            else if(strcmp(argv[1], "4") == 0){
                machine = initAppareil("machine 4\0", IP_1, IP_2, UDP_1);
                serveur = initServ(UDP_4);
            }
            else {
                perror("Erreur : Argument IP invalide\n");
                return 1;
            }
        }
        // Boucle réseau
        else if(strcmp(argv[2], "-l") == 0) {
            printf("Boucle locale\n");

            if(strcmp(argv[1], "1") == 0){
                machine = initAppareil("machine 1\0", IP_1, IP_2, UDP_2);
                serveur = initServ(UDP_1);
            }
            else if(strcmp(argv[1], "2") == 0){
                machine = initAppareil("machine 2\0", IP_2, IP_3, UDP_3);
                serveur = initServ(UDP_2);
            }
            else if(strcmp(argv[1], "3") == 0){
                machine = initAppareil("machine 3\0", IP_3, IP_4, UDP_4);
                serveur = initServ(UDP_3);
            }
            else if(strcmp(argv[1], "4") == 0){
                machine = initAppareil("machine 4\0", IP_4, IP_1, UDP_1);
                serveur = initServ(UDP_4);
            }
            else {
                perror("Erreur : Argument IP invalide");
                return 1;
            }
        }
        else{
            printf("Argument %s incorrect\n", argv[2]);
            perror("Erreur : argument de tpye de reseau incorrect\n");
            return EXIT_FAILURE;
        }
    } 
    else if(argc > 3) {
        perror("Erreur : trop d'arguments !\n");
        return EXIT_FAILURE;
    }

    else if (argc < 2){
        perror("Erreur : il manque des arguments !\n");
        return EXIT_FAILURE;    
    }

    else{
        printf("Boucle local\n");
        if(strcmp(argv[1], "1") == 0){
            machine = initAppareil("machine 1\0", IP_1, IP_2, UDP_2);
            serveur = initServ(UDP_1);
        }
        else if(strcmp(argv[1], "2") == 0){
            machine = initAppareil("machine 2\0", IP_2, IP_3, UDP_3);
            serveur = initServ(UDP_2);
        }
        else if(strcmp(argv[1], "3") == 0){
            machine = initAppareil("machine 3\0", IP_3, IP_4, UDP_4);
            serveur = initServ(UDP_3);
        }
        else if(strcmp(argv[1], "4") == 0){
            machine = initAppareil("machine 4\0", IP_4, IP_1, UDP_1);
            serveur = initServ(UDP_4);
        }
        else {
            perror("Erreur : Argument IP invalide");
            return 1;
        }
    }

    printf("Je suis la machine %s\n", getIP(machine));

    char* liste[] = {IP_1, IP_2, IP_3, IP_4};

    if(startSession(machine, serveur, liste)){
        perror("Erreur : session non démarrée\n");
        return EXIT_FAILURE;
    }
    closeServ(serveur);
    deleteServ(serveur);
    deleteAppareil(machine);
    

    return EXIT_SUCCESS;
}


void lire_IP() 
{
    printf("Entrez votre @IP : ");
    scanf("%s", IP_1);
    strncat(IP_1, "\0", 1);

    printf("Entrez l'@IP du suivant : ");
    scanf("%s", IP_2);
    strncat(IP_2, "\0", 1);

    printf("Entrez @IP 3 : ");
    scanf("%s", IP_3);
    strncat(IP_3, "\0", 1);

    printf("Entrez @IP 4 : ");
    scanf("%s", IP_4);
    strncat(IP_4, "\0", 1);

    getchar();
}