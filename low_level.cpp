#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <Eigen/Dense>
#include <iostream>
#include <random>
#include <chrono>
#include <string>

// Alias pour JSON
using json = nlohmann::json;

class Task {
public:
    int identifier;
    int size;
    Eigen::MatrixXd a; // Matrice A avec Eigen
    Eigen::VectorXd b; // Vecteur b avec Eigen
    Eigen::VectorXd x; // Résultat
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

        a.resize(size, size);
        b.resize(size);
        x.resize(size);

        for (int j = 0; j < size; ++j) {
            for (int i = 0; i < size; ++i) {
                // a[i + j * size] = dist(gen); // Matrice linéarisée
            }
            b[j] = dist(gen); // Vecteur
        }
    }

    // Méthode pour effectuer un travail : résolution de Ax = b
    void work() {
        //Eigen::MatrixXd matrix_a(size, size);
        //Eigen::VectorXd vector_b(size);

        // Convertir le tableau 1D en matrice Eigen
        /*for (int j = 0; j < size; ++j) {
            for (int i = 0; i < size; ++i) {
                matrix_a(i, j) = a(i + j * size);
            }
            vector_b(j) = b[j];
        }*/

        // Résoudre Ax = b
        
        auto start = std::chrono::high_resolution_clock::now();
        x = a.colPivHouseholderQr().solve(b);
        auto end = std::chrono::high_resolution_clock::now();

        time = std::chrono::duration<double>(end - start).count();
        std::cout << "Temps de calcul: " << time << " secondes." << std::endl;

        // Stocker le résultat dans x
        /*for (int i = 0; i < size; ++i) {
            x[i] = result(i);
        }*/
    }

    // Convertir l'objet en JSON
    json to_json() const {
        // Convertir la matrice a (Eigen::MatrixXd) en un tableau 1D (std::vector<double>)
        std::vector<double> a_flat;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                a_flat.push_back(a(i, j));  // Ajouter chaque élément de la matrice a
            }
        }

        // Convertir le vecteur b (Eigen::VectorXd) en std::vector<double>
        std::vector<double> b_vec(b.data(), b.data() + b.size());

        // Convertir le vecteur x (Eigen::VectorXd) en std::vector<double>
        std::vector<double> x_vec(x.data(), x.data() + x.size());

        return json{
            {"identifier", identifier},
            {"size", size},
            {"a", a_flat}, // Matrice a convertie en tableau linéarisé
            {"b", b_vec},  // Vecteur b
            {"x", x_vec},  // Vecteur x
            {"time", time}
        };
    }


    // Charger un objet Task depuis JSON avec vérification de type
    Task(const json& j) {
        std::cout << 10 << std::endl;




        //std::cout << 12 << std::endl;
        // Afficher le contenu brut du JSON pour le diagnostic
        //std::cout << "Raw JSON received: " << j.dump(4) << std::endl;

        // Vérification du type avant d'assigner la valeur
        if (j.contains("identifier") && j["identifier"].is_number()) {
                //std::cout << 13 << std::endl;
            identifier = j["identifier"];
                    //std::cout << 14 << std::endl;
        } else {
                //std::cout << 15 << std::endl;
            std::cerr << "Error: 'identifier' should be a number, but it's of type " << j["identifier"].type_name() << std::endl;
            throw std::invalid_argument("'identifier' must be a number.");
        }
        //std::cout << 16 << std::endl;
        if (j.contains("size") && j["size"].is_number()) {
                //std::cout << 17 << std::endl;
            size = j["size"];
                    //std::cout << 18 << std::endl;
        } else {
                //std::cout << 19 << std::endl;
            std::cerr << "Error: 'size' should be a number, but it's of type " << j["size"].type_name() << std::endl;
            throw std::invalid_argument("'size' must be a number.");
        }
                //std::cout << 20  << std::endl;
        a.resize(size, size);
        if (j.contains("a") && j["a"].is_array()) {
            //std::cout << 30  << std::endl;
            for(int k = 0; k < size; k++) {
                //std::cout << "k: "<< k  << std::endl;
                    for(int z = 0; z < size; z++) {
                        //std::cout << "z: "<< z  << std::endl;
                        a(k,z) = j["a"][k][z];
                        //std::cout << "OK"  << std::endl;
                    }
            }
        } else {
            std::cerr << "Error: 'a' should be an array of numbers, but it's of type " << j["a"].type_name() << std::endl;
            throw std::invalid_argument("'a' must be an array of numbers.");
        }
                //std::cout << 21 << std::endl;
        b.resize(size);
        if (j.contains("b") && j["b"].is_array()) {
            for(int k = 0; k < size; k++) {
            b(k) = j["b"][k];}
        } else {
            std::cerr << "Error: 'b' should be an array of numbers, but it's of type " << j["b"].type_name() << std::endl;
            throw std::invalid_argument("'b' must be an array of numbers.");
        }
                //std::cout << 22 << std::endl;
        
        x.resize(size);
        if (j.contains("x") && j["x"].is_array()) {
            for(int k = 0; k < size; k++) {
            x(k) = j["x"][k];}
                
        } else {
            std::cerr << "Error: 'x' should be an array of numbers, but it's of type " << j["x"].type_name() << std::endl;
            throw std::invalid_argument("'x' must be an array of numbers.");
        }
                //std::cout << 23 << std::endl;
        if (j.contains("time") && j["time"].is_number()) {
            time = j["time"];
        } else {
            std::cerr << "Error: 'time' should be a number, but it's of type " << j["time"].type_name() << std::endl;
            throw std::invalid_argument("'time' must be a number.");
        }
                //std::cout << 24 << std::endl;
    }
};

int main() {
    // Configuration du proxy
    cpr::Url proxy_url = "http://localhost:8000";
    //std::cout << 0 << std::endl;

    // Requête HTTP GET via le proxy
    auto response = cpr::Get(proxy_url);
    //std::cout << 1 << std::endl;
    // Vérifier la réponse de GET
    if (response.status_code == 200) {
       // std::cout << "Response from server:\n" << response.text << std::endl;
    //std::cout << 2 << std::endl;
        try {
            // Parse JSON response
                //std::cout << 3<< std::endl;
            nlohmann::json json_data = nlohmann::json::parse(response.text);
           // std::cout << "JSON parsed: " << json_data.dump(4) << std::endl;
    //std::cout << 4 << std::endl;
            // Créer un objet Task à partir de la réponse JSON
            Task task(json_data);
    //std::cout << 5 << std::endl;
            // Effectuer le travail (résolution de Ax = b)
            task.work();
                //std::cout << 6 << std::endl;

            // Sérialiser la tâche traitée en JSON
            json task_json = task.to_json();
            std::string json_str = task_json.dump(4);
                //std::cout << 7 << std::endl;

            //std::cout << "\nTask JSON to be sent:\n" << json_str << std::endl;

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

        } catch (const std::exception& e) {
            std::cerr << "Error processing the task: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Failed to connect: " << response.error.message << std::endl;
    }

    return 0;
}

