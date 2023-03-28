typedef struct Appareil Appareil;
struct Appareil;

Appareil* initAppareil(char* parametre[], int nb_parametre);
char* getNom(Appareil* a);
char* getIP(Appareil* a);
char* getIPSuivant(Appareil* a);
void deleteAppareil(Appareil* a);
int getUDPport(Appareil *a);