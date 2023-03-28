#include "headers/Client.h"
#include "headers/Serveur.h"
#include "headers/Packet.h"
#include "headers/Appareil.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define UDP_port 8000

int main(int argc, char* argv[])
{
    Appareil* a = initAppareil(argv, argc);

    if(a == NULL){
        return EXIT_FAILURE;
    }

    int nb_boucle = 0;
    packet* p = createPacket("Message de début\0", a);   
    Serveur* s = initServ(UDP_port);
    setData(p, "Salut beau gosse\0");
    if(argc > 1){
        sendData(p, a);
    }

    while(nb_boucle < 10)
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