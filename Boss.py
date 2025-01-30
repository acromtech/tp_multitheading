from task import Task
from QueueClient import QueueClient
import time
import matplotlib.pyplot as plt


class Boss(QueueClient):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.num_tasks = 0  # Suivi du nombre total de tâches envoyées
        self.task_sizes = []  # Stockage des tailles des tâches
        self.execution_times = []  # Stockage des temps d'exécution

    def add_tasks(self, num_tasks=10, sizes=None):
        if sizes is None:
            sizes = [
                100,
                200,
                500,
                800,
                1000,
                2000,
                3000,
                4000,
                5000,
                6000,
            ]  # Liste des tailles des tâches

        self.num_tasks = num_tasks  # Mémoriser combien de tâches ont été envoyées
        print("Boss is adding tasks with varying sizes.")

        for i in range(num_tasks):
            size = sizes[i % len(sizes)]  # Sélectionne une taille cycliquement
            self.task_sizes.append(size)  # Stocker la taille de la tâche

            task = Task(identifier=i, size=size)

            # Mesurer le temps pour envoyer les tâches
            start_time = time.time()

            self.task_queue.put(task)
            print(f"Task {task.identifier} with size {task.size} sent.")

            elapsed_time = time.time() - start_time
            print(f"Task {task.identifier} dispatched in {elapsed_time:.2f} seconds.")

        print("All tasks have been added.")

    def get_results(self):
        """Attend et récupère tous les résultats envoyés par Minion ou low_level"""
        print("\nBoss is collecting results...")

        results_received = 0
        while results_received < self.num_tasks:
            if not self.result_queue.empty():
                task = self.result_queue.get()
                results_received += 1
                self.execution_times.append(task.time)  # Stocker le temps d'exécution
                print(f"Task {task.identifier} completed in {task.time:.2f} seconds.")

        print("\nAll tasks have been processed.")

        # Sauvegarder les résultats dans un fichier texte
        self.save_results_to_file("performance_results.txt")

        self.plot_results()  # Générer le graphique

    def save_results_to_file(self, filename="performance_results.txt"):
        """Sauvegarde les résultats dans un fichier texte"""
        with open(filename, "w") as f:
            f.write("identifier,size,time\n")  # Entête CSV
            for i in range(len(self.task_sizes)):
                f.write(f"{i},{self.task_sizes[i]},{self.execution_times[i]}\n")
        print(f"Résultats sauvegardés dans {filename}")

    def plot_results(self):
        """Affiche un graphique des tailles de tâches vs temps d'exécution"""
        plt.figure(figsize=(8, 5))
        plt.scatter(
            self.task_sizes, self.execution_times, color="b", label="Temps d'exécution"
        )
        plt.plot(
            self.task_sizes, self.execution_times, linestyle="-", color="b", marker="o"
        )

        plt.xlabel("Taille de la tâche")
        plt.ylabel("Temps d'exécution (s)")
        plt.title("Temps d'exécution en fonction de la taille des tâches")
        plt.legend()
        plt.grid(True)
        plt.show()


if __name__ == "__main__":
    boss = Boss()
    boss.add_tasks(num_tasks=10)
    boss.get_results()  # Récupère dynamiquement les résultats
