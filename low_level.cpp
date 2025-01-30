#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <Eigen/Dense>
#include <iostream>
#include <chrono>
#include <omp.h>

using json = nlohmann::json;

class Task {
public:
    int identifier;
    int size;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> x;
    double time;

    Task(int id = 0, int s = 0) : identifier(id), size(s), time(0.0) {}

    static Task from_json(const json& j) {
        Task t(j["identifier"], j["size"]);

        // Convertir `a` de matrice 2D en tableau 1D
        if (j["a"].is_array() && j["a"][0].is_array()) {
            int s = t.size;
            t.a.resize(s * s);
            for (int i = 0; i < s; ++i) {
                for (int j_idx = 0; j_idx < s; ++j_idx) {
                    t.a[i * s + j_idx] = j["a"][i][j_idx];
                }
            }
        } else {
            t.a = j["a"].get<std::vector<double>>();
        }

        // Vérifier et charger `b`
        if (j["b"].is_array()) {
            t.b = j["b"].get<std::vector<double>>();
        } else {
            throw std::runtime_error("Erreur: 'b' n'est pas un tableau.");
        }

        t.x.resize(t.size);
        return t;
    }

    json to_json() const {
        return json{
            {"identifier", identifier},
            {"size", size},
            {"time", time}
        };
    }

    void work() {
        Eigen::MatrixXd matrix_a(size, size);
        Eigen::VectorXd vector_b(size);

        Eigen::setNbThreads(4); // Activer OpenMP

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                matrix_a(i, j) = a[i * size + j];
            }
            vector_b(i) = b[i];
        }

        auto start = std::chrono::high_resolution_clock::now();
        Eigen::VectorXd result = matrix_a.partialPivLu().solve(vector_b);
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double>(end - start).count();

        for (int i = 0; i < size; ++i) {
            x[i] = result(i);
        }
    }
};

int main() {
    while (true) {
        std::cout << "En attente d'une tâche depuis Boss..." << std::endl;

        // Récupérer une tâche via HTTP
        cpr::Response response = cpr::Get(cpr::Url{"http://localhost:8000"});

        if (response.status_code == 204 || response.text.empty()) {
            std::cout << "Aucune tâche disponible, arrêt du programme." << std::endl;
            break; // Sortir proprement de la boucle
        }

        if (response.status_code != 200) {
            std::cerr << "Erreur: Impossible de récupérer la tâche. Code HTTP: "
                      << response.status_code << std::endl;
            break; // Sortir pour éviter une boucle infinie
        }

        // Parser la tâche reçue
        json received_json = json::parse(response.text);
        Task task = Task::from_json(received_json);

        std::cout << "Tâche reçue ! ID: " << task.identifier << ", Taille: " << task.size << std::endl;

        // Exécuter la tâche
        task.work();
        std::cout << "Temps de calcul pour la tâche " << task.identifier << " : " 
                  << task.time << " secondes." << std::endl;

        // Envoyer le résultat via HTTP POST
        json result_json = task.to_json();
        cpr::Response post_response = cpr::Post(
            cpr::Url{"http://localhost:8000"},
            cpr::Header{{"Content-Type", "application/json"}},
            cpr::Body{result_json.dump()}
        );

        if (post_response.status_code == 200) {
            std::cout << "Résultat envoyé avec succès au Boss !" << std::endl;
        } else {
            std::cerr << "Erreur lors de l'envoi du résultat. Code HTTP: "
                      << post_response.status_code << std::endl;
        }
    }

    return 0;
}
