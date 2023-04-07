#ifndef _APPAREIL_H
#define _APPAREIL_H

typedef struct Appareil Appareil;
struct Appareil;

Appareil* initAppareil(char* nom, char *ip, char *ip_suivante, int port);
char* getNom(Appareil* a);
char* getIP(Appareil* a);
char* getIPSuivant(Appareil* a);
void deleteAppareil(Appareil* a);
int getUDPport(Appareil *a);

#endif