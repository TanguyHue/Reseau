# TP Réseau

L'objectif de ce TP est de mettre en place un réseau local en C entre plusieurs machines. 

## Auteurs

Le projet a été réalisé par :
* **Nicolas Godard**
* **Emilien L'Haridon**
* **Leo Bigardi**
* **Tanguy Hue**

## Installation

Pour installer le projet, il faut cloner le dépôt git et lancer le script d'installation.

```bash
git clone https://github.com/TanguyHue/Reseau.git
cd Reseau
make
```

## Utilisation

Pour lancer le programme, il faut lancer la commande suivante :

```bash
./main [appareil] [port(optionel)]
```

Liste des appareils possibles :
* `1` Appareil 1 : @IP = 172.19.70.26
* `2` Appareil 2 : @IP = 172.19.70.27
* `3` Appareil 3 : @IP = 172.19.70.28
* `l` localhost : @IP = 127.0.0.1

On peut changer les adresses IP dans le fichier **Appareil.c**, en changeant les valeurs de la constante **IP_1**, **IP_2** et **IP_3**.

Le port est optionnel, il est par défaut à 8000.

## Organisation du projet

Le projet est composé de 2 dossiers :
* **sources** : contient les fichiers sources du projet
* **headers** : contient les fichiers headers du projet

Ces dossiers sont composés de 4 fichiers : 
* **Appareil.c** : contient le code source d'un appareil
* **Client.c** : contient le code source du client
* **Serveur.c** : contient le code source du serveur
* **Packet.c** : contient le code source des paquets

Les **serveurs** permettent d'écouter sur un port particulier, tandis que les **clients** permettent d'envoyer des paquets sur le port spécifié dans l'appareil.

### Appareil

L'appareil est le point de départ du programme. Il permet de choisir l'appareil que l'on souhaite utiliser, ainsi que le port sur lequel on souhaite communiquer.

Pour cela, on utilise une structure **Appareil** qui contient :
* **nom** : le nom de l'appareil
* **IP** : l'adresse IP de l'appareil
* **IP_suivant** : l'adresse IP de l'appareil suivant dans l'anneau
* **UDP_port** : le port UDP sur lequel on souhaite communiquer

### Client

Le client permet d'envoyer des paquets sur le port spécifié dans l'appareil.

Cette bliothèque n'utilise pas de structure particulière, elle utilise uniquement des fonctions, notamment la fonction **sendData** qui permet d'envoyer des données sur le port spécifié.

### Serveur

Le serveur permet d'écouter sur un port particulier.

Pour cela, on utilise une structure **Serveur** qui contient :
* **udp_socket** : le port sur lequel on souhaite écouter
* **sa_Serv** : la structure sockaddr_in qui contient les informations sur le serveur
* **sa_Client** : la structure sockaddr_in qui contient les informations sur le client
* **taille_sa** : la taille de la structure sockaddr_in

### Packet

Le packet permet de créer des paquets qui seront envoyés sur le réseau.

Pour cela, on utilise une structure **Packet** qui contient :
* **size** : la taille des données
* **adress_emetteur** : l'adresse IP de l'émetteur
* **adress_destinataire** : l'adresse IP du destinataire
* **data** : les données
* **checksum** : le checksum du paquet

La taille maximale d'un paquet est de 1024 octets. On peut changer cette taille en modifiant la constante **PACKET_SIZE** dans le fichier **Packet.c**.

Enfin, grâce à la fonction **tokenPacket**, on peut créer un token qui sera envoyé sur le réseau. Ce token ne contient pas de données, mais uniquement l'adresse IP de l'émetteur et du destinataire.