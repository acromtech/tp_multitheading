from task import Task
from QueueClient import QueueClient
import time

class Boss(QueueClient):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def add_tasks(self, num_tasks=10):
        print("Boss is adding tasks.")
        for i in range(num_tasks):
            task = Task(identifier=i, size=1000)  # Crée une tâche de taille fixe (exemple : 10)
            
            # Crée deux copies identiques de la tâche
            task_for_python = task
            task_for_cpp = Task(identifier=i, size=task.size)
            task_for_cpp.a = task.a  # Copie les données matricielles
            task_for_cpp.b = task.b

            # Mesurer le temps pour envoyer les tâches
            start_time = time.time()

            # Envoyer au Minion (Python)
            self.task_queue.put(task_for_python)
            print(f"Task {task.identifier} sent to Minion.")

            # Envoyer à low_level (C++)
            self.task_queue.put(task_for_cpp)
            print(f"Task {task.identifier} sent to low_level.")

            elapsed_time = time.time() - start_time
            print(f"Task {task.identifier} dispatched in {elapsed_time:.2f} seconds.")

        print("All tasks have been added.")

if __name__ == "__main__":
    boss = Boss()
    boss.add_tasks(num_tasks=10)

