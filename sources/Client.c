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
#include <sys/types.h>
#include <signal.h>

#define BUFFER_SIZE 256

// Adresses IP des appareils
char IP1[128];
char IP2[128];
char IP3[128];
char IP4[128];

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
// Délai d'attente pour les transmissions de paquet, en microseconde
#define DELAI 20000

// Création d'un procesus pour la saisie de l'utilisateur
void createProcess();
// Analyse de la saisie de l'utilisateur et l'envoi du paquet
void message();

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

int startSession(Appareil* a, Serveur* s, char* IP[]){
    strcpy(IP1, IP[0]);
    strcpy(IP2, IP[1]);
    strcpy(IP3, IP[2]);
    strcpy(IP4, IP[3]);

    machine = a;
    serveur = s;

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


    deletePacket(p);
    return EXIT_SUCCESS;
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
            setAdressDest(data, IP1);
            break;
        case '2':
            setAdressDest(data, IP2);
            break;
        case '3':
            setAdressDest(data, IP3);
            break;
        case '4':
            setAdressDest(data, IP4);
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