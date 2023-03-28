#include "headers/Client.h"
#include "headers/Serveur.h"
#include "headers/Packet.h"
#include "headers/Appareil.h"
#define UDP_port 8000

int main(int argc, char* argv[])
{
    Appareil* a = initAppareil(argv, argc);

    if(a == NULL){
        return EXIT_FAILURE;
    }

    int nb_boucle = 0;
    packet* p = createPacket("Salut ! \0", a);   
    Serveur* s = initServ(UDP_port);
    setData(p, "Coucou toi...\0");
    if(argc > 1){
        sendData(p, a);
    }

    while(1)
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

    return 0;
}