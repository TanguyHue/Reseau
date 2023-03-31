#include "headers/Client.h"
#include "headers/Serveur.h"
#include "headers/Packet.h"
#include "headers/Appareil.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>

#define IP_1 "127.0.0.1\0"
#define IP_2 "127.0.0.2\0"
#define IP_3 "127.0.0.3\0"
#define IP_4 "127.0.0.4\0"

int main(int argc, char* argv[])
{
    Appareil* machine;
    Serveur* serveur;

    
    int n = 0;

    pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return EXIT_FAILURE;
	} else if (pid == 0) {
        machine = initAppareilParam("2\0", IP_2, IP_3, 8002);
        serveur = initServ(8001);
        n = 2;
	} else {
        pid_t pid2 = fork();
        if(pid2 == -1){
            perror("fork");
            return EXIT_FAILURE;
        }
        else if(pid2 == 0) {
            machine = initAppareilParam("3\0", IP_3, IP_4, 8003);
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
                machine = initAppareilParam("4\0", IP_4, IP_1, 8000);
                serveur = initServ(8003);
                n = 4;
            }

            else{
                machine = initAppareilParam("1\0", IP_1, IP_2, 8001);
                serveur = initServ(8000);
                n = 1;
            }
        }
	}

    packet* p = createPacket("Salut", machine);

    if(n == 1){
        printf("PPID : %d\n", getppid());
        printf("PID : %d\n", getpid());
        sleep(2);
        printf("Je suis la machine 1\n");
        setAdressEmetteur(p, IP_1);
        setAdressDest(p, IP_3);
        sendData(p, machine);
        printf("Message envoyé à : %s\n", getIPSuivant(machine));
        printf("====================================\n");
    }

    while(1){
        receipt(serveur, p);
        sleep(3);
        printf("Je suis la machine %s\n", getIP(machine));
        if(checkIP(machine, p)){
            printf("/!\\ Je suis le destinataire /!\\ \n");
            switch (n)
            {
            case 1:
                setAdressEmetteur(p, IP_1);
                setAdressDest(p, IP_3);
                break;
            
            case 2:
                setAdressEmetteur(p, IP_2);
                setAdressDest(p, IP_4);
                break;
            
            case 3:
                setAdressEmetteur(p, IP_3);
                setAdressDest(p, IP_2);
                break;

            case 4:
                setAdressEmetteur(p, IP_4);
                setAdressDest(p, IP_1);
                break;
            
            default:
                break;
            }
        }
        printf("Le message est à destination de : %s\n", getAdressDest(p));
        printf("Le message contient : %s\n", getData(p));
        sendData(p, machine);
        printf("Message envoyé à : %s\n", getIPSuivant(machine));
        printf("=====================================\n");
    }

    closeServ(serveur);

    deletePacket(p);
    deleteServ(serveur);
    deleteAppareil(machine);
    

    return EXIT_SUCCESS;
}