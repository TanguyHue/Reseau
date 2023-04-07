#include "../headers/Appareil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define IP_1 "172.19.70.26\0"
#define IP_2 "172.19.70.27\0"
#define IP_3 "172.19.70.28\0"

struct Appareil{
    char* nom;
    char* IP;
    char* IP_suivant;
    int UDP_port;
};

/*
    * Fonction qui initialise un appareil
    * @param parametre[] tableau de chaîne de caractère contenant l'emplacement de l'appareil dans l'anneau et le port UDP. Mettre l pour être en local
    * @param nb_parametre nombre d'éléments dans le tableau parametre[]
    * @return Appareil* pointeur sur l'appareil initialisé
*/
Appareil* initAppareil(char* parametre[], int nb_parametre){    
    if(nb_parametre < 2 || nb_parametre > 3){
        if(nb_parametre == 1){
            printf("Indiquer le numéro de l'appareil\n");
        }
        else{
            printf("Trop d'arguments\n");
        }
        return NULL;
    }

    Appareil* a = (Appareil*)malloc(sizeof(Appareil));
    
    a->nom = (char*)malloc(sizeof(char) * 16);
    a->IP = (char*)malloc(sizeof(char) * 16);
    a->IP_suivant = (char*)malloc(sizeof(char) * 16);

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
        strcpy(a->IP, IP_1);
        strcpy(a->IP_suivant, IP_2);
        printf("Client 1 : %s\n", a->IP);
    }
    else if(strcmp(parametre[1], "2") == 0){
        strcpy(a->nom, "2\0");
        strcpy(a->IP, IP_2);
        strcpy(a->IP_suivant, IP_3);
        printf("Client 2 : %s\n", a->IP);
    }
    else if(strcmp(parametre[1], "3") == 0){
        strcpy(a->nom, "3\0");
        strcpy(a->IP, IP_3);
        strcpy(a->IP_suivant, IP_1);
        printf("Client 3 : %s\n", a->IP);
    }
    else{
        printf("Client inconnu\n");
    }
    return a;
}

/*
    * Fonction qui initialise un appareil
    * @param nom nom de l'appareil
    * @param ip adresse IP de l'appareil
    * @param ip_suivante adresse IP de l'appareil suivant dans l'anneau
    * @return Appareil* pointeur sur l'appareil initialisé
*/
Appareil *initAppareilParam(char* nom, char *ip, char *ip_suivante, int port)
{
    Appareil* a = (Appareil*)malloc(sizeof(Appareil));

    a->nom = (char*)malloc(sizeof(char) * 6);
    a->IP = (char*)malloc(sizeof(char) * 16);
    a->IP_suivant = (char*)malloc(sizeof(char) * 16);
    a->UDP_port = port;

    strcpy(a->nom, nom);
    strcpy(a->IP, ip);
    strcpy(a->IP_suivant, ip_suivante);
    return a;
}
/*
 * Fonction qui retourne le nom de l'appareil
 * @param a pointeur sur l'appareil
 * @return char* nom de l'appareil
 */
char *getNom(Appareil *a)
{
    return a->nom;
}

/*
    * Fonction qui retourne l'adresse IP de l'appareil
    * @param a pointeur sur l'appareil
    * @return char* adresse IP de l'appareil
*/
char *getIP(Appareil *a)
{
    return a->IP;
}

/*
    * Fonction qui retourne l'adresse IP de l'appareil suivant dans l'anneau
    * @param a pointeur sur l'appareil
    * @return char* adresse IP de l'appareil suivant
*/
char *getIPSuivant(Appareil *a)
{
    return a->IP_suivant;
}

/*
    * Fonction qui retourne le port UDP de l'appareil
    * @param a pointeur sur l'appareil
    * @return int port UDP de l'appareil
*/
int getUDPport(Appareil *a)
{
    return a->UDP_port;
}

/*
    * Fonction qui libère la mémoire allouée pour l'appareil
    * @param a pointeur sur l'appareil
*/
void deleteAppareil(Appareil *a)
{
    free(a->nom);
    free(a->IP);
    free(a->IP_suivant);
    free(a);
}