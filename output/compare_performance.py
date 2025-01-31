import matplotlib.pyplot as plt


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


# Charger les données des trois fichiers
identifiers_python, sizes_python, times_python = load_performance_data(
    "performance_results_python.txt"
)
identifiers_cpp_debug, sizes_cpp_debug, times_cpp_debug = load_performance_data(
    "performance_results_cpp_debug.txt"
)
identifiers_cpp_release, sizes_cpp_release, times_cpp_release = load_performance_data(
    "performance_results_cpp_release.txt"
)

# Tracer les performances
plt.figure(figsize=(8, 5))
plt.plot(
    sizes_python,
    times_python,
    linestyle="-",
    marker="o",
    color="b",
    label="Minion.py (Python)",
)
plt.plot(
    sizes_cpp_debug,
    times_cpp_debug,
    linestyle="--",
    marker="s",
    color="r",
    label="low_level.cpp (C++ - Debug)",
)
plt.plot(
    sizes_cpp_release,
    times_cpp_release,
    linestyle="-",
    marker="^",
    color="g",
    label="low_level.cpp (C++ - Release)",
)

# Configuration du graphique
plt.xlabel("Taille de la tâche")
plt.ylabel("Temps d'exécution (s)")
plt.title("Comparaison des performances Python vs C++ (Debug vs Release)")
plt.legend()
plt.grid(True)

# Afficher le graphique
plt.show()
