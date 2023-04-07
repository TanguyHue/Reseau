#define _XOPEN_SOURCE 700
#include "headers/Client.h"
#include "headers/Serveur.h"
#include "headers/Packet.h"
#include "headers/Appareil.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

// Ports d'écoute des différents appareils
#define UDP_1 8001
#define UDP_2 8002
#define UDP_3 8003
#define UDP_4 8004

// Délai d'attente pour les transmissions de paquet, en microseconde
#define DELAI 20000

// Adresses IP des appareils
char IP_1[128] = "127.0.0.1\0";
char IP_2[128] = "127.0.0.2\0";
char IP_3[128] = "127.0.0.3\0";
char IP_4[128] = "127.0.0.4\0";

// Pipe de processus
int fd[2];
// Buffer du pipe
char buffer[1048];
// Processus fils créé
pid_t pid_fils;
Appareil* machine;
packet* p;
Serveur* serveur;
int nb_token = 0;

// 172.19.65.111 <- 1
// 172.19.65.112 <- 2
// 172.19.65.114 <- 3
// 172.19.65.115 <- 4

// Création d'un procesus pour la saisie de l'utilisateur
void createProcess();
// Analyse de la saisie de l'utilisateur et l'envoi du paquet
void message();
// Saisie des IP des machines, pour les réseaux externes
void lire_IP();

int main(int argc, char* argv[])
{   
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

    p = createPacket("default", machine);

    createProcess();

    while(1){
        receipt(serveur, p);
        usleep(DELAI);
        
        if(checkToken(p)){
                nb_token = 1;
                usleep(DELAI);
                sendData(p, machine);
        }
        else{
            if(checkIP(machine, p)){
                kill(pid_fils, SIGKILL);
                printf("\n\n/!\\ Message reçu /!\\ \n");
                if(checksum(p) == 0){
                    printf("Le message est incorrect !\n");
                }
                else{
                    printf("Message reçu de : %s\n", getAdressEmetteur(p));
                    printf("Le message est : %s\n", getData(p));
                }
                createProcess();
                packet* token = tokenPacket();
                sendData(token, machine);
                nb_token = 1;
                sleep(2);
                deletePacket(token);
            }
            else{
                sendData(p, machine);
                usleep(DELAI);
            }
        }
        
    }

    closeServ(serveur);

    deletePacket(p);
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

void createProcess(){

    if(pipe(fd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_fils = fork();
    if (pid_fils == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid_fils == 0) {
        close(fd[0]);

        memset(buffer, 0, 1048);
        printf("\n> ");
        scanf("%[^\n]", buffer);
        
        write(fd[1], &buffer, 1048);
        kill(getppid(), SIGUSR1);
    } else {
        struct sigaction si_statut;
        sigaction(SIGUSR1, NULL, &si_statut);
        si_statut.sa_handler = message;
        si_statut.sa_flags = SA_RESTART;
        sigaction(SIGUSR1, &si_statut, NULL);
        close(fd[1]);
    }
}

void message(){
    if (read(fd[0], buffer, 1048) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    char message[1048];
    memcpy(message, buffer+1, 1047);
    message[strlen(buffer)] = '\0';

    packet* data = createPacket(message, machine);
    int n = 1;

    switch(buffer[0]){
        case '1':
            setAdressDest(data, IP_1);
            break;
        case '2':
            setAdressDest(data, IP_2);
            break;
        case '3':
            setAdressDest(data, IP_3);
            break;
        case '4':
            setAdressDest(data, IP_4);
            break;

        case 't':
            if(nb_token){
                printf("\nIl y a déjà un token\n");
            }
            else{
                packet* token = tokenPacket();
                sendData(token, machine);
                printf("\nToken envoyé !\n");
                nb_token = 1;
                printf("Nb token : 1\n");
                deletePacket(token);
            }
            usleep(DELAI);
            n = 0;
            break;

        default:
            printf("Erreur de saisie ! Mettez le numéro de l'appareil en premier caractère puis le message\n");
            kill(pid_fils, SIGKILL);
            usleep(DELAI);
            createProcess();
            return;
    } 

    if(n) { printf("\nMessage : %s\n", message); }
    setAdressEmetteur(data, getIP(machine));
    if(nb_token == 0 && n) { printf("Attention, il n'y a pas de token de boucle\n"); }
    if(n) { nb_token = -1; }
    
    // Changer, mettre plutot une valeur à une variable et cest le while principal qui va envoyer le message
    if(n){
        printf("En attente de token...\n");
        receipt(serveur, p);
        printf("Token reçu !\n");
        sendData(data, machine);
        nb_token = 0;
        n = 0;
    }
    kill(pid_fils, SIGKILL);

    usleep(DELAI);
    if(nb_token){
        packet* token = tokenPacket();
        sendData(token, machine);
        deletePacket(token);
    }

    deletePacket(data);
    createProcess();
}