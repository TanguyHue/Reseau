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

// 172.19.65.111 <- 1
// 172.19.65.112 <- 2
// 172.19.65.114 <- 3
// 172.19.65.115 <- 4

char IP_1[128] = "127.0.0.1\0";
char IP_2[128] = "127.0.0.2\0";
char IP_3[128] = "127.0.0.3\0";
char IP_4[128] = "127.0.0.4\0";

void createProcess();
void message();
int fd[2];
char buffer[1048];
pid_t pid_fils;
Appareil* machine;
packet* p;
Serveur* serveur;
int reset = 0;
int nb_token = 0;
int attente = 0;

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

int main(int argc, char* argv[])
{
    if(argc < 2){
        printf("Il manque des arguments !\n");
        return -1;    
    }
    else if (argc == 2){
        if(strcmp(argv[1], "1") == 0){
            machine = initAppareilParam("machine 1\0", IP_1, IP_2, 8001);
            serveur = initServ(8000);
        }
        else if(strcmp(argv[1], "2") == 0){
            machine = initAppareilParam("machine 2\0", IP_2, IP_3, 8002);
            serveur = initServ(8001);
        }
        else if(strcmp(argv[1], "3") == 0){
            machine = initAppareilParam("machine 3\0", IP_3, IP_4, 8003);
            serveur = initServ(8002);
        }
        else if(strcmp(argv[1], "4") == 0){
            machine = initAppareilParam("machine 4\0", IP_4, IP_1, 8000);
            serveur = initServ(8003);
        }
        else {
            printf("Erreur : Argument IP invalide");
            return 1;
        }
    }
    else {
        if(strcmp(argv[1], "1") == 0){
            machine = initAppareilParam("machine 1\0", IP_1, IP_2, 8001);
            serveur = initServ(8004);
        }
        else if(strcmp(argv[1], "2") == 0){
            machine = initAppareilParam("machine 2\0", IP_1, IP_2, 8002);
            serveur = initServ(8001);
        }
        else if(strcmp(argv[1], "3") == 0){
            machine = initAppareilParam("machine 3\0", IP_1, IP_2, 8003);
            serveur = initServ(8002);
        }
        else if(strcmp(argv[1], "4") == 0){
            machine = initAppareilParam("machine 4\0", IP_1, IP_2, 8004);
            serveur = initServ(8003);
        }
        else {
            printf("Erreur : Argument IP invalide");
            return 1;
        }
        
        // Boucle locale
        if(strcmp(argv[2], "-l") == 0) {

        }
        // Boucle réseau
        else if(strcmp(argv[2], "-r") == 0) {
            lire_IP();
        }
        else {
            printf("Erreur : trop d'arguments !\n");
            return 1;
        }
    }

    printf("Je suis la machine %s\n", getIP(machine));

    p = createPacket("default", machine);

    struct sigaction si_statut;
    sigaction(SIGUSR1, NULL, &si_statut);
    si_statut.sa_handler = message;
    si_statut.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &si_statut, NULL);

    createProcess();

    while(1){
        receipt(serveur, p);
        usleep(20000);
        
        if(checkToken(p)){
            if(reset != 0){
                nb_token = 0;
                printf("Nb token : 0\n");
                if(reset == 2){
                    sendData(p, machine);
                }
                reset = 0;
                printf("Reset : 0\n");
            }
            else{
                nb_token = 1;
                usleep(20000);
                sendData(p, machine);
            }
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
                if(checkReset(p, machine)){
                    if(checkReset(p, machine) == 1){
                        reset = 2;
                        nb_token = 0;
                    }
                    sendData(p, machine);
                    usleep(20000);
                }
            }
        }
        
    }

    closeServ(serveur);

    deletePacket(p);
    deleteServ(serveur);
    deleteAppareil(machine);
    

    return EXIT_SUCCESS;
}
/*
    Fonction de création de procesus
*/
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

        char temp[1048];
        memset(temp, 0, 1048);
        printf("\n> ");
        scanf("%[^\n]", temp);
        
        write(fd[1], &temp, 1048);
        kill(getppid(), SIGUSR1);
    } else {
        close(fd[1]);
    }
}

void message(){
    int nbytes = read(fd[0], buffer, 1048);
    if (nbytes == -1) {
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
                reset = 0;
                //printf("Reset : 0\n");
                deletePacket(token);
            }
            //usleep(2000);
            n = 0;
            break;
        
        case 'r':
            attente = 1;
            if(reset == 0 && nb_token){
                reset = 1;
                //printf("Reset : 1\n");
                printf("\nSuppresion du token\n");
                packet* pReset = resetPacket(machine);
                sendData(pReset, machine);
                deletePacket(pReset);
            }
            else{
                printf("\nIl n'y a pas de token ou il y a déjà une demande de reset\n");
            }
            usleep(2000);
            attente = 0;
            n = 0;
            break;

        default:
            printf("Message pipe : %s\n", buffer);
            sleep(5);
            printf("Erreur de saisie ! Mettez le numéro de l'appareil en premier caractère puis le message\n");
            kill(pid_fils, SIGKILL);
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

    usleep(200);
    if(nb_token){
        packet* token = tokenPacket();
        sendData(token, machine);
        deletePacket(token);
    }

    deletePacket(data);
    createProcess();
}