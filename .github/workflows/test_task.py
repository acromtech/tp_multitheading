import unittest
import numpy.testing as npt
from task import Task


class TestTask(unittest.TestCase):
    def test_matrix_multiplication(self):
        size = 10
        task = Task(size=size)
        task.work()

        # Vérification des résultats
        npt.assert_allclose(
            task.a @ task.x,
            task.b,
            rtol=1e-5,
            atol=0,
            err_msg="Erreur : la solution calculée n'est pas correcte",
        )


if __name__ == "__main__":
    unittest.main()
