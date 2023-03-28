#include "headers/Client.h"
#include "headers/Serveur.h"
#include "headers/Packet.h"
#include "headers/Appareil.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[])
{
    // Initialisation de l'appareil
    Appareil* a = initAppareil(argv, argc);

    if(a == NULL){
        return EXIT_FAILURE;
    }

    int nb_boucle = 0;
    
    // Initialisation du paquet
    packet* p = createPacket("Message de début\0", a); 
    // Initialisation du serveur  
    Serveur* s = initServ(getUDPport(a));

    // Envoi du premier message si on est le premier appareil de l'anneau
    if(strcmp(getNom(a), "1") == 0 || strcmp(getNom(a), "local") == 0){
        setData(p, "Salut beau gosse\0");
        sendData(p, a);
    }

    while(nb_boucle < 5)
    {
        receipt(s, p);
        printf("Nombre de boucle : %d\n", nb_boucle);
        nb_boucle++;
        printf("Message envoyé : %s\n", getData(p));
        sendData(p, a);
        sleep(1);
    }

    closeServ(s);

    deletePacket(p);
    deleteServ(s);
    deleteAppareil(a);

    return EXIT_SUCCESS;
}