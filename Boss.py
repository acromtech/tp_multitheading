from task import Task
from QueueClient import QueueClient
import time

class Boss(QueueClient):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def add_tasks(self, num_tasks=10, sizes=None):
        if sizes is None:
            sizes = [100, 200, 500, 800, 1000, 2000, 3000, 4000, 5000, 6000]  # Liste des tailles des tâches
            
        print("Boss is adding tasks with varying sizes.")
        for i in range(num_tasks):
            size = sizes[i % len(sizes)]  # Sélectionne une taille cycliquement

            task = Task(identifier=i, size=size)

            # Mesurer le temps pour envoyer les tâches
            start_time = time.time()

            self.task_queue.put(task)
            print(f"Task {task.identifier} with size {task.size} sent.")

            elapsed_time = time.time() - start_time
            print(f"Task {task.identifier} dispatched in {elapsed_time:.2f} seconds.")

        print("All tasks have been added.")

if __name__ == "__main__":
    boss = Boss()
    boss.add_tasks(num_tasks=10)

