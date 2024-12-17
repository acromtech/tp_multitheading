import json
import time
import numpy as np


class Task:
    def __init__(self, identifier=0, size=None):
        self.identifier = identifier
        # Choose the size of the problem
        self.size = size or np.random.randint(300, 3_000)
        # Generate the input of the problem
        self.a = np.random.rand(self.size, self.size)
        self.b = np.random.rand(self.size)
        # Prepare room for the results
        self.x = np.zeros((self.size))
        self.time = 0

    def work(self):
        start = time.perf_counter()
        self.x = np.linalg.solve(self.a, self.b)
        self.time = time.perf_counter() - start

    def to_json(self) -> str:
        """Convert the object's data to a JSON string."""
        data = {
            "identifier": self.identifier,
            "size": self.size,
            "a": self.a.tolist(),
            "b": self.b.tolist(),
            "x": self.x.tolist(),
            "time": self.time,
        }
        return json.dumps(data, indent=4)

    @staticmethod
    def from_json(text: str) -> "Task":
        """Create a Task object from a JSON string."""
        data = json.loads(text)  # Parse the JSON string
        # Create a new Task object
        task = Task(identifier=data["identifier"], size=data["size"])
        # Reconstruct the object's state
        task.a = np.array(data["a"])
        task.b = np.array(data["b"])
        task.x = np.array(data["x"])
        task.time = data["time"]
        return task

    def __eq__(self, other: "Task") -> bool:
        if not isinstance(other, Task):
            return False
        print(f"Comparing identifiers: {self.identifier} vs {other.identifier}")
        print(f"Comparing sizes: {self.size} vs {other.size}")
        print(f"Comparing a: {np.array_equal(self.a, other.a)}")
        print(f"Comparing b: {np.array_equal(self.b, other.b)}")
        print(f"Comparing x: {np.array_equal(self.x, other.x)}")
        print(f"Comparing time: {self.time} vs {other.time}")
        return (
            self.identifier == other.identifier
            and self.size == other.size
            and np.array_equal(self.a, other.a)
            and np.array_equal(self.b, other.b)
            and np.array_equal(self.x, other.x)
            and self.time == other.time
        )



if __name__ == "__main__":
    
    # TEST POUR to_json
    task = Task(identifier=1, size=10)
    task.work()
    json_output = task.to_json()
    print(json_output)
    
    # TEST POUR from_json
    # Créer une tâche, la convertir en JSON, puis la recréer à partir du JSON
    task = Task(identifier=1, size=10)
    task.work()
    json_output = task.to_json()

    # Recréer un objet Task à partir du JSON
    task_from_json = Task.from_json(json_output)

    # Vérification
    print(task_from_json.identifier)  # 1
    print(task_from_json.size)        # 10
    print(task_from_json.time)        # Temps mesuré
    
    # TEST POUR __eq__
    task1 = Task(identifier=1, size=10) # Meme task
    task2 = Task(identifier=1, size=10) # Meme task

    # Exécuter le travail pour qu'ils aient des résultats différents
    task1.work()
    task2.work()

    # Vérifier l'égalité (devrait être False car le travail modifie `time` et `x`)
    print(task1 == task2)  # False

    # Comparer avant d'exécuter `work`
    task3 = Task(identifier=1, size=10)
    print(task1 == task3)  # True (avant que `work` ne soit appelé)

