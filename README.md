# Projet Réseau 📶

L'objectif de ce TP est de mettre en place un réseau local en C entre plusieurs machines. 

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