#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <Eigen/Dense>
#include <iostream>
#include <random>
#include <chrono>
#include <string>

// Alias pour JSON
using json = nlohmann::json;

// Classe Task
#include <vector>
#include <random>
#include <chrono>
#include <nlohmann/json.hpp>
#include <Eigen/Dense>

// Alias pour JSON
using json = nlohmann::json;

class Task {
public:
    int identifier;
    int size;
    std::vector<double> a; // Matrice stockée sous forme linéarisée
    std::vector<double> b; // Vecteur b
    std::vector<double> x; // Résultat
    double time;           // Temps de calcul

    // Constructeur
    Task(int id = 0, int s = 0) : identifier(id), size(s), time(0.0) {
        if (size > 0) {
            generate_random_data();
        }
    }

    // Générer des données aléatoires pour a et b
    void generate_random_data() {
        std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<> dist(0.0, 1.0);

        a.resize(size * size);
        b.resize(size);
        x.resize(size);

        for (int j = 0; j < size; ++j) {
            for (int i = 0; i < size; ++i) {
                a[i + j * size] = dist(gen); // Matrice linéarisée
            }
            b[j] = dist(gen); // Vecteur
        }
    }

    // Méthode pour effectuer un travail : résolution de Ax = b
    void work() {
        Eigen::MatrixXd matrix_a(size, size);
        //std::cout << "\nOK3:\n"  std::endl;
        Eigen::VectorXd vector_b(size);
        //std::cout << "\nOK4\n"  std::endl;

        // Convertir le tableau 1D en matrice Eigen
        for (int j = 0; j < size; ++j) {
            for (int i = 0; i < size; ++i) {
                matrix_a(i, j) = a[i + j * size];
                //std::cout << "\nOK6:\n"  std::endl;
            }
            vector_b(j) = b[j];
            //std::cout << "\nOK5:\n"  std::endl;
        }

        // Résoudre Ax = b
        auto start = std::chrono::high_resolution_clock::now();
        Eigen::VectorXd result = matrix_a.colPivHouseholderQr().solve(vector_b);
        auto end = std::chrono::high_resolution_clock::now();

        time = std::chrono::duration<double>(end - start).count();

        // Stocker le résultat dans x
        for (int i = 0; i < size; ++i) {
            x[i] = result(i);
        }
    }

    // Convertir l'objet en JSON
    json to_json() const {
        return json{
            {"identifier", identifier},
            {"size", size},
            {"a", a}, // Tableau linéarisé
            {"b", b},
            {"x", x},
            {"time", time}
        };
    }

    // Charger un objet Task depuis JSON
    static Task from_json(const std::string& json_text) {
        Task t;
        json j = json::parse(json_text);

        t.identifier = j.at("identifier");
        t.size = j.at("size");
        t.a = j.at("a").get<std::vector<double>>();
        t.b = j.at("b").get<std::vector<double>>();
        t.x = j.at("x").get<std::vector<double>>();
        t.time = j.at("time");

        return t;
    }
};



int main() {
    // Configuration du proxy
    cpr::Url proxy_url = "http://localhost:8080";

    // Requête HTTP GET via le proxy
    auto response = cpr::Get(cpr::Url{"http://httpbin.org/get"}
                             );

    // Vérifier la réponse de GET
    if (response.status_code == 200) {
        std::cout << "Response from server:\n" << response.text << std::endl;

        // Parse JSON response
        nlohmann::json json_data = nlohmann::json::parse(response.text);
        std::cout << "JSON parsed: " << json_data.dump(4) << std::endl;
    } else {
        std::cerr << "Failed to connect: " << response.error.message << std::endl;
    }

    // ----- Ajout de la logique Task -----
    //std::cout << "\nOK1:\n"  std::endl;

    // Créer un objet Task
    Task task(1, 10); // ID = 1, Taille = 10
    task.work();      // Résoudre Ax = b

    // Sérialiser en JSON
    json task_json = task.to_json();
    //std::cout << "\nOK1:\n"  std::endl;
    std::string json_str = task_json.dump(4);
    //std::cout << "\nOK2:\n"  std::endl;

    std::cout << "\nTask JSON to be sent:\n" << json_str << std::endl;

    // Envoyer l'objet Task via POST
    auto post_response = cpr::Post(
        cpr::Url{"http://localhost:8000"},
        cpr::Body{json_str},
        cpr::Header{{"Content-Type", "application/json"}}
    );

    // Vérifier la réponse de POST
    if (post_response.status_code == 200) {
        std::cout << "\nPOST Response from server:\n" << post_response.text << std::endl;

        
    } else {
        std::cerr << "Failed to send Task data: " << post_response.error.message << std::endl;
    }

    return 0;
}
