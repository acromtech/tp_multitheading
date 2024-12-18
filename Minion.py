from task import Task
from QueueClient import QueueClient
import time

class Minion(QueueClient):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def work(self):
        print("Minion started working.")
        while True:
            try:
                # Récupérer une tâche depuis la file d’attente
                task = self.task_queue.get(timeout=10)
                print(f"Processing task {task.identifier} of size {task.size}.")

                # Mesurer le temps de traitement
                start_time = time.time()
                task.work()  # Effectue la résolution Ax = b
                elapsed_time = time.time() - start_time

                # Envoyer le résultat dans la file de résultats
                self.result_queue.put(task)
                print(f"Task {task.identifier} completed in {elapsed_time:.2f} seconds.")
            except Exception as e:
                print("No more tasks or an error occurred:", e)
                break

if __name__ == "__main__":
    minion = Minion()
    minion.work()

