# **Multi-Threading Task Manager**

[Teachings](https://nim65s.github.io/teach/)

## **Objectif**

Ce projet vise à comparer les performances du calcul parallèle entre **Python** et **C++** pour la résolution de systèmes linéaires \(Ax = b\), en utilisant :
- **Python** pour le prototypage rapide et la gestion des tâches.
- **C++** pour les performances optimisées sur de grandes matrices.

## **Description**

Le projet met en place un **gestionnaire de tâches multi-threading** basé sur un système client-serveur :
- Un **Boss** distribue les tâches.
- Des **Minions** les exécutent.
- Un **Proxy HTTP** facilite la communication client-serveur.
- Les clients C++ effectuent des calculs à hautes performances.

## **Architecture**

### **Structure des fichiers :**
- **Python :**
  - `Boss.py` : Envoie les tâches.
  - `Minion.py` : Exécute les tâches.
  - `QueueManager.py` : Gère les files d'attente.
  - `proxy.py` : Interface HTTP pour interagir avec le système.
  - `task.py` : Structure d'une tâche (matrices).
  - `test_task.py` : Tests unitaires.
- **C++ :**
  - `low_level.cpp` : Résolution de systèmes linéaires avec Eigen et communication HTTP via cpr.
  - `client.cpp` : Exemple basique de client utilisant un proxy.

## **Prérequis**

### **Python**
- Python 3.x
- Bibliothèques :
  - NumPy
  - Unittest (pour les tests)
  - HTTPX (pour le proxy)

### **C++**
- Un compilateur compatible avec **C++17** (GCC/Clang).
- Bibliothèques :
  - [Eigen](https://eigen.tuxfamily.org)
  - [nlohmann/json](https://github.com/nlohmann/json)
  - [cpr](https://github.com/libcpr/cpr)
  - OpenMP (pour le parallélisme)
- **CMake ≥ 3.14** pour la configuration.

## **Installation**

### **1. Dépendances**
Exécute le script `setup.sh` pour installer toutes les dépendances nécessaires :

**setup.sh :**
```bash
#!/bin/bash
echo "Installation des dépendances..."

# Python
uv add numpy httpx

# C++ Libraries
sudo apt-get update
sudo apt-get install -y cmake g++ libssl-dev nlohmann-json3-dev

echo "Dépendances installées avec succès."
```

Exécution :
```bash
chmod +x setup.sh
./setup.sh
```

## **Compilation et Exécution**

### **1. Côté Python**

#### **Démarrer le gestionnaire de files :**
```bash
python QueueManager.py
```

#### **Envoyer des tâches :**
```bash
python Boss.py
```

#### **Lancer les travailleurs :**
```bash
python Minion.py
```

#### **Lancer le Proxy HTTP :**
```bash
python proxy.py
```
Par défaut, le proxy écoute sur le **port 8000**.

### **2. Côté C++**

#### **Compilation :**
```bash
cmake -B build -S .
cmake --build build
```

#### **Exécution du client principal :**
```bash
./build/low_level
```

#### **Exécution du client proxy :**
```bash
./build/client
```

## **Résultats**

Nous comparons les performances entre Python et C++ pour la résolution de systèmes linéaires de différentes tailles de matrices.

### **Exemple des commandes et résultats :**

#### **Python (Task.py avec NumPy) :**
```bash
python3 task.py
```
- **Matrices de taille 1000 :** Temps ≈ **2.3 secondes**
- **Matrices de taille 10000 :** Temps ≈ **45 secondes**

#### **C++ (low_level.cpp avec Eigen) :**
```bash
./build/low_level
```
- **Matrices de taille 1000 :** Temps ≈ **0.5 secondes**
- **Matrices de taille 10000 :** Temps ≈ **7 secondes**

---

## **Analyse des Performances**

| Taille de la Matrice | Python (NumPy) | C++ (Eigen) |
|----------------------|---------------:|------------:|
| **1000x1000**        | 2.3 s          | 0.5 s       |
| **10000x10000**      | 45 s           | 7 s         |

### **Conclusion**

- **C++** est **environ 6 fois plus rapide** que Python pour les petites matrices et **plus de 6 fois plus rapide** pour les grandes matrices.
- Cette différence s'explique par :
  - L'optimisation de **Eigen** en C++.
  - Le coût de l'interprétation en Python.
  - L'utilisation d'OpenMP pour le calcul parallèle en C++.

## **Optimisations apportées**

1. **CMake :**
   - Désactivation du **mode debug** en définissant `CPR_ENABLE_SSL OFF` pour réduire le temps de compilation.

   ```cmake
   option(CPR_ENABLE_SSL "" OFF)
   ```

2. **OpenMP** pour paralléliser les calculs en C++.

3. **Scripts automatisés :**
   - Ajout d'un `setup.sh` pour simplifier l'installation.

## **Conclusion**

Ce projet démontre la différence de performance entre Python et C++ dans un environnement multi-threading. Python est utile pour le prototypage rapide, mais **C++** est essentiel pour des calculs intensifs et des performances optimisées. 🚀

## **Auteurs**

- **Alexis Gibert**
- **Nour Ghiloufi**  
Projet réalisé dans le cadre des enseignements de multithreading de l'UPSSITECH.
