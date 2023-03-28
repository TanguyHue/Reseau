#include "../headers/Appareil.h"
#include <stdio.h>
#include "../headers/Appareil.h"
#include <stdlib.h>
#include <string.h>

struct Appareil{
    char* nom;
    char* IP;
    char* IP_suivant;
    int UDP_port;
};

Appareil* initAppareil(char* parametre[], int nb_parametre){
    Appareil* a = (Appareil*)malloc(sizeof(Appareil));
    
    a->nom = (char*)malloc(sizeof(char) * 6);
    a->IP = (char*)malloc(sizeof(char) * 16);
    a->IP_suivant = (char*)malloc(sizeof(char) * 16);
    
    if(nb_parametre < 2 || nb_parametre > 3){
        if(nb_parametre == 1){
            printf("Indiquer le numéro de l'appareil\n");
        }
        else{
            printf("Trop d'arguments\n");
        }
        return NULL;
    }

    if(nb_parametre == 3){
        a->UDP_port = atoi(parametre[2]);
    }
    else{
        a->UDP_port = 8000;
    }
    
    if(strcmp(parametre[1], "l") == 0){
        strcpy(a->nom, "local\0");
        strcpy(a->IP, "127.0.0.1\0");
        strcpy(a->IP_suivant, "127.0.0.1\0");
        printf("Client local : %s\n", a->IP);
    }
    else if(strcmp(parametre[1], "1") == 0){
        strcpy(a->nom, "1\0");
        strcpy(a->IP, "172.19.70.26\0");
        strcpy(a->IP_suivant, "172.19.70.27\0");
        printf("Client 1 : %s\n", a->IP);
    }
    else if(strcmp(parametre[1], "2") == 0){
        strcpy(a->nom, "2\0");
        strcpy(a->IP, "172.19.70.27\0");
        strcpy(a->IP_suivant, "172.19.70.28\0");
        printf("Client 2 : %s\n", a->IP);
    }
    else if(strcmp(parametre[1], "3") == 0){
        strcpy(a->nom, "3\0");
        strcpy(a->IP, "172.19.70.28\0");
        strcpy(a->IP_suivant, "172.19.70.29\0");
        printf("Client 3 : %s\n", a->IP);
    }
    else{
        printf("Client inconnu\n");
    }
    return a;
}

char *getNom(Appareil *a)
{
    return a->nom;
}

char *getIP(Appareil *a)
{
    return a->IP;
}

char *getIPSuivant(Appareil *a)
{
    return a->IP_suivant;
}

int getUDPport(Appareil *a)
{
    return a->UDP_port;
}

void deleteAppareil(Appareil *a)
{
    free(a->nom);
    free(a->IP);
    free(a->IP_suivant);
    free(a);
}