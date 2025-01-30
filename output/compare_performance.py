import matplotlib.pyplot as plt
import numpy as np

def load_performance_data(filename):
    """Charge les données de performance depuis un fichier texte."""
    identifiers, sizes, times = [], [], []
    with open(filename, "r") as f:
        next(f)  # Ignorer l'en-tête
        for line in f:
            identifier, size, time = line.strip().split(",")
            identifiers.append(int(identifier))
            sizes.append(int(size))
            times.append(float(time))
    return identifiers, sizes, times

# Charger les données des deux fichiers
identifiers_python, sizes_python, times_python = load_performance_data("performance_results_python.txt")
identifiers_cpp, sizes_cpp, times_cpp = load_performance_data("performance_results_cpp.txt")

# Tracer les performances
plt.figure(figsize=(8, 5))
plt.plot(sizes_python, times_python, linestyle='-', marker='o', color='b', label="Minion.py (Python)")
plt.plot(sizes_cpp, times_cpp, linestyle='-', marker='s', color='r', label="low_level.cpp (C++)")

# Configuration du graphique
plt.xlabel("Taille de la tâche")
plt.ylabel("Temps d'exécution (s)")
plt.title("Comparaison des performances Python vs C++")
plt.legend()
plt.grid(True)

# Afficher le graphique
plt.show()

