Faire une petite conclusion du tp, dire que utiliser python est plus lent que le cpp (montrer via des copie terminal)
Faire un peu de tri sur le repo
Vérifier que tout est a jour
Faire un setup.sh avec les dep

# Multi-Threading Task Manager

[Teachings](https://nim65s.github.io/teach/)

## Description

Ce projet est une implémentation d'un gestionnaire de tâches multi-threading basé sur un système client-serveur. Il simule un environnement où un **Boss** distribue des tâches, des **Minions** les traitent, et un **Proxy** assure une interface HTTP pour interagir avec le système. La communication repose sur des files d'attente partagées.

## Architecture

### Structure des fichiers :
- **Boss.py** : Envoie les tâches dans la file d'attente pour que les Minions puissent les traiter.
- **Minion.py** : Récupère les tâches depuis la file d'attente et les exécute.
- **QueueManager.py** : Met en place un gestionnaire de files pour gérer les tâches et les résultats.
- **QueueClient.py** : Classe de base pour les clients connectés au **QueueManager**.
- **proxy.py** : Serveur HTTP permettant l'envoi et la récupération de tâches via des requêtes HTTP.
- **task.py** : Définit la structure d'une tâche avec un problème mathématique (résolution d'un système linéaire).
- **test_task.py** : Tests unitaires pour valider la résolution des tâches.
- **low_level.cpp / client.cpp** : Composants C++ pour la gestion bas-niveau et des clients.
- 
## Fonctionnement

### 1. **Initialisation du Gestionnaire de Files**
Lancer le `QueueManager.py` pour démarrer le serveur des files d'attente.

```bash
python QueueManager.py
```

### 2. **Ajout des Tâches**
Le fichier `Boss.py` ajoute des tâches à la file de tâches.

```bash
python Boss.py
```

### 3. **Exécution des Tâches**
Les **Minions** récupèrent les tâches depuis la file et les traitent.

```bash
python Minion.py
```

### 4. **Proxy HTTP**
Le **Proxy** permet d'interagir avec le système via des requêtes HTTP :
- **GET** : Récupérer une tâche en cours.
- **POST** : Envoyer une tâche terminée.

```bash
python proxy.py
```

Par défaut, le Proxy écoute sur le port `8000`.

## Exemples d'Utilisation

### Récupérer une tâche via HTTP :
```bash
curl http://127.0.0.1:8000
```

### Envoyer une tâche terminée :
```bash
curl -X POST -H "Content-Type: application/json" -d '{"identifier":1,"size":100}' http://127.0.0.1:8000
```

## Tests

Pour valider le fonctionnement des tâches :

```bash
python test_task.py
```

## Prérequis

- **Python 3.x**
- **Bibliothèques** :
    - NumPy
    - Unittest (pour les tests)
- **C++ Compilateur** (pour les fichiers `client.cpp` et `low_level.cpp` si nécessaires)

## Auteur

**Alexis GIBERT / Nour GHILOUFI** - Projet de gestion de tâches multi-threading pour l'UPSSITECH.
