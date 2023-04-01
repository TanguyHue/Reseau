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

#define IP_1 "127.0.0.1\0"
#define IP_2 "127.0.0.2\0"
#define IP_3 "127.0.0.3\0"
#define IP_4 "127.0.0.4\0"

void createProcess();
void quitte();
void reprendre();
void message();
int fd[2];
char buffer[1048];
pid_t pid_fils;
Appareil* machine;

int main(int argc, char* argv[])
{
    Serveur* serveur;


    /*
    int n = 0;

    pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return EXIT_FAILURE;
	} else if (pid == 0) {
        machine = initAppareilParam("machine 2\0", IP_2, IP_3, 8002);
        serveur = initServ(8001);
        n = 2;
	} else {
        pid_t pid2 = fork();
        if(pid2 == -1){
            perror("fork");
            return EXIT_FAILURE;
        }
        else if(pid2 == 0) {
            machine = initAppareilParam("machine 3\0", IP_3, IP_4, 8003);
            serveur = initServ(8002);
            n = 3;
        }
        else{
            pid_t pid3 = fork();
            if(pid3 == -1){
                perror("fork");
                return EXIT_FAILURE;
            }
            else if(pid3 == 0){
                machine = initAppareilParam("machine 4\0", IP_4, IP_1, 8000);
                serveur = initServ(8003);
                n = 4;
            }

            else{
                machine = initAppareilParam("machine 1\0", IP_1, IP_2, 8001);
                serveur = initServ(8000);
                n = 1;
            }
        }
	}*/

    if(argc < 2){
        printf("Il manque des arguments !\n");
        return -1;    
    }
    else{
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
    }

    printf("Je suis la machine %s\n", getIP(machine));

    packet* p = createPacket("default", machine);

    if(strcmp(getIP(machine), IP_1) == 0){
        //sleep(2);
        //printf("Je suis la machine 1\n");
        //packet* token = tokenPacket(machine);
        //sendData(token, machine);
    }

    struct sigaction si_statut;
    sigaction(SIGUSR1, NULL, &si_statut);
    si_statut.sa_handler = message;
    si_statut.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &si_statut, NULL);

    createProcess();
    //printf("J'ai récupéré le pid : %d\n", pid_fils);

    while(1){
        receipt(serveur, p);
        //sleep(1);
        if(checkToken(p)){
            printf("Token détecté !\n");   
            printf("Message émis de : %s\n", getAdressEmetteur(p)); 
            sendData(p, machine);
        }
        else{
            //sleep(1);
            //printf("\n\n");
            //printf("Je suis la machine %s\n", getIP(machine));
            //sleep(1);
            if(checkIP(machine, p)){
                printf("\n\n/!\\ Je suis le destinataire /!\\ \n");
                if(checksum(p) == 0){
                    printf("Le message est incorrect !!!!!!!!!!!\n");
                }
                else{
                    printf("Le message est correct !\n");
                    printf("Le message est : %s\n", getData(p));
                }
                fflush(stdout);
                kill(pid_fils, SIGUSR2);
            }
            else{
                sendData(p, machine);
                //kill(pid_fils, SIGUSR2);
            }
            /*
            //sleep(1);
            printf("Le message est à destination de : %s\n", getAdressDest(p));
            printf("Le message contient : %s\n", getData(p));
            //sendData(p, machine);
            //sleep(1);
            printf("Message envoyé à : %s\n", getIPSuivant(machine));
            printf("=====================================\n");*/
            sleep(1);
        }
        fflush(stdout);
    }

    closeServ(serveur);

    deletePacket(p);
    deleteServ(serveur);
    deleteAppareil(machine);
    

    return EXIT_SUCCESS;
}

void createProcess(){

    if(pipe(fd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_fils = fork();
    //printf("pid : %d\n", pid_fils);
    if (pid_fils == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid_fils == 0) {
        close(fd[0]);

        struct sigaction si_statut;
        sigaction(SIGUSR1, NULL, &si_statut);
        si_statut.sa_handler = quitte;
        si_statut.sa_flags = SA_RESTART;
        sigaction(SIGUSR1, &si_statut, NULL);

        struct sigaction si_statut_2;
        sigaction(SIGUSR2, NULL, &si_statut_2);
        si_statut_2.sa_handler = reprendre;
        si_statut_2.sa_flags = SA_RESTART;
        sigaction(SIGUSR2, &si_statut_2, NULL);

        char temp[1048];
        memset(temp, 0, 1048);
        printf("> ");
        fflush(stdout); // assure que le message de prompt est imprimé
        fgets(temp, 1048, stdin); // lecture non bloquante
        
        write(fd[1], &temp, 1048);
        kill(getppid(), SIGUSR1);
        while(1);
    } else {
        close(fd[1]);
    }
}

void quitte(){
    //printf("\nJe quitte !\n");
    //pause();
}

void reprendre(){
    //printf("\nJe reprends !\n");
    sleep(1);
    char temp[1048];
    memset(temp, 0, 1048);
    printf("> ");
    fflush(stdout); // assure que le message de prompt est imprimé
    fgets(temp, 1048, stdin); // lecture non bloquante
    printf("\n");
    
    write(fd[1], &temp, 1048);
    kill(getppid(), SIGUSR1);
}

void message(){
    //sleep(1);
    int nbytes = read(fd[0], buffer, 1048);
    if (nbytes == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    //printf("Données reçues : %s\n", buffer);

    char message[1048];
    memcpy(message, buffer+1, 1047);
    message[strlen(buffer)] = '\0';
    printf("Message : %s\n", message);
    packet* data = createPacket(message, machine);

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

        default:
            printf("Erreur de saisie !\n");
            kill(pid_fils, SIGUSR2);
            return;
    }

    setAdressEmetteur(data, getIP(machine));
    //printf("Message à envoyer : %s\n", getData(data));
    //printf("Message à envoyer à : %s\n", getAdressDest(data));
    //printf("Message envoyé par : %s\n", getAdressEmetteur(data));
    sendData(data, machine);
    //sleep(1);
    kill(pid_fils, SIGUSR2);
}