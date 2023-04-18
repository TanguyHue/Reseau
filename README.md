# Projet Réseau 📶

L'objectif de ce TP est de mettre en place un réseau local en C entre plusieurs machines. 

## Auteurs

Le projet a été réalisé par :
* **Nicolas Godard**
* **Emilien L'Haridon**
* **Leo Brigardis**
* **Tanguy Hue**

## Compilation

Pour compiler le projet, il faut cloner le dépôt git et lancer le script d'installation.

```bash
git clone https://github.com/TanguyHue/Reseau.git
cd Reseau
make
```

## Utilisation

Pour lancer le programme, il faut lancer la commande suivante :

```bash
./main [appareil] [type reseau(local ou reseau)]
```

Le type de réseau est optionnel, il est par défaut à `local`.
Il existe 2 types de réseau :
* **local** : le réseau est local, les appareils sont sur la même machine. Pour cela, on utilise le paramètre `-l`.
* **reseau** : le réseau est externe, les appareils sont sur des machines différentes. Pour cela, on utilise le paramètre `-r`.

Une fois le programme compiler, voici les étapes à suivre pour utiliser le programme :
* L'utilisateur choisit l'appareil qu'il souhaite utiliser, en utilisant la commande `./main [appareil]`
* Sur l'un des appareils, on envoie un token sur le réseau, en envoyant le message `t`
* Vous pouvez envoyez des messages sur le réseau, en envoyant le message `[appareilDestinataire][message]`

Les ports utilisés sont les suivants :
* **8011** : pour l'appareil 1
* **8012** : pour l'appareil 2
* **8013** : pour l'appareil 3
* **8014** : pour l'appareil 4

Il est possible de changer les ports dans le fichier **main.c**, en changeant les valeurs de la constante **PORT_1**, **PORT_2**, **PORT_3** et **PORT_4**.

### Local

Liste des appareils possibles :
* `1` Appareil 1 : @IP_1 = 127.0.0.1
* `2` Appareil 2 : @IP_2 = 127.0.0.2
* `3` Appareil 3 : @IP_3 = 127.0.0.3
* `4` Appareil 4 : @IP_4 = 127.0.0.4

On peut changer les adresses IP dans le fichier **main.c**, en changeant les valeurs de la constante **IP_1**, **IP_2**, **IP_3** et **IP_4**.  


### Réseau externe

En mode **réseau externe**, le programme va demander à l'utilisateur l'adresse IP de sa machine, et l'adresse IP de la machine suivante dans l'anneau. Il demandera ensuite les deux dernières adresses IP de l'anneau, et les machines sont donc : 

* `1` Appareil 1 : La machine qui execute le programme
* `2` Appareil 2 : la machine suivante dans l'anneau
* `3` Appareil 3 : @IP_3
* `4` Appareil 4 : @IP_4

## Organisation du projet

Le projet est composé de 2 dossiers :
* **sources/** : contient les fichiers sources du projet
* **headers/** : contient les fichiers headers du projet

Ces dossiers sont composés de 4 fichiers : 
* **Appareil.c** : contient le code source d'un appareil
* **Client.c** : gère l'écoute et l'émission de paquets
* **Serveur.c** : gère la réception de paquets
* **Packet.c** : contient le code source des paquets

### Appareil

L'appareil est le point de départ du programme. Il permet de choisir l'appareil que l'on souhaite utiliser, ainsi que le port sur lequel on souhaite communiquer.

Pour cela, on utilise une structure **Appareil** qui contient :
* **nom** : le nom de l'appareil
* **IP** : l'adresse IP de l'appareil
* **IP_suivant** : l'adresse IP de l'appareil suivant dans l'anneau
* **UDP_port** : le port UDP sur lequel on souhaite communiquer

### Client

Le client permet d'envoyer des paquets sur le port spécifié dans l'appareil, et gère l'écoute sur l'anneau. 
L'envoi se fait à l'aide de la fonction `sendData` et l'écoute de l'anneau et de l'entrée standard se fait à l'aide de la fonction `startSession`.

Pour cela, le client utilise 2 threads :
* Le thread principal ou "père" va créer le processus fils à l'aide d'un `fork`, puis il va écouter sur le réseau à l'aide de la fonction `receipt` de `Serveur.h`.
* Le thread fils va attendre que l'utilisateur rentre un message. Quand il reçoit un message, il envoie un signal **SIGUSR1** au processus père, qui va envoyer le message sur le réseau. Une fois que le message est envoyé, le processus fils va mourir, et va être recrée. Cela est fait pour gérer le buffer de sortie et améliorer l'affichage.

Pour l'envoi du message, le client va convertir le paquet à envoyer sous forme d'une chaines de caractères, et ensuite il va envoyer le message sur le réseau.

Lorsqu'un message est reçu, le processus principal va vérifier : 
* Si le message est un token ou un message. Si c'est un token, il va regarder si un reset a été demandé par une machine, dans ce cas il va mettre son nombre de token a 0 et envoyé le token à la machine d'après
* Sinon, il va regarder si le message est pour lui, si c'est le cas, il va tuer son processus fils et afficher le message. 
* Sinon, il va regarder si c'est un reset. Si c'est le cas, il va mettre son nombre de token à 0 et changer la variable reset et envoyé le token à la machine d'après.
* Sinon il va renvoyé le paquet à la machine d'après.


### Serveur

Le serveur permet de gérer la réception des paquets sur le port spécifié dans l'appareil.

Pour cela, on utilise une structure **Serveur** qui contient :
* **udp_socket** : le port sur lequel on souhaite écouter
* **sa_Serv** : la structure sockaddr_in qui contient les informations sur le serveur
* **sa_Client** : la structure sockaddr_in qui contient les informations sur le client
* **taille_sa** : la taille de la structure sockaddr_in

Lorsque le serveur reçoit un message, il va devoir le traiter en convertissant la chaine de caractère reçu sous forme de paquet.

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

## Fonctionnement

![fonctionnement du programme](img/fonctionnement.png)

### Structure d'un paquet

Lorsqu'un paquet est envoyé, il est écrit sous la forme suivante d'une chaine de caractères :

```bash
[SIZE_ADRESS_EMETTEUR][ADRESS_EMETTEUR][SIZE_ADRESS_DEST][ADRESS_DEST][SIZE_DATA][DATA][CHECKSUM]
```

Tous les entiers sont écrits sur 4 bits, et les bits non utilisés sont remplis par des `0`.

Les paquets de type **token** : ils ont comme adresse de destination "-1"  et du destinataire "-1", et comme port -1. Ils sont utilisés pour savoir quel appareil peut envoyer un message. Ils sont déterminer à l'aide de la fonction `checkErrorCheckSum` de la bibliothèque **Packet.h**. Pour cela, on vérifie si la taille du paquet est à 0 ou non.
