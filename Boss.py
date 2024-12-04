# Boss doit envoyer les taches au QueueClient pour que les Minions les réalisent
from task import Task
from QueueClient import QueueClient
import time 

class Boss(QueueClient):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def add_tasks(self, num_tasks=10):
        print("Boss is adding tasks.")
        for i in range(num_tasks):
            task = Task(identifier=i)
            print(f"Adding task {task.identifier} of size {task.size}.")
            self.task_queue.put(task)
        print("All tasks have been added.")



if __name__ == "__main__":
    boss = Boss()
    boss.add_tasks(num_tasks=10)
    boss.get_results()

