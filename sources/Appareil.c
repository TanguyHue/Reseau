#include "../headers/Appareil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Appareil{
    char* nom;
    char* IP;
    char* IP_suivant;
    int UDP_port;
};

/*
    * Fonction qui initialise un appareil
    * @param nom nom de l'appareil
    * @param ip adresse IP de l'appareil
    * @param ip_suivante adresse IP de l'appareil suivant dans l'anneau
    * @param port port d'écoute de l'appareil suivant
    * @return Appareil* pointeur sur l'appareil initialisé
*/
Appareil *initAppareil(char* nom, char *ip, char *ip_suivante, int port)
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
    * Fonction qui retourne le port d'écoute de l'appareil suivantinitAppareilParam dans l'anneau
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