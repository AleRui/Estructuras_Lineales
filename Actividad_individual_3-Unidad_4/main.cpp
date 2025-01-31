// ficheros de cabecera estándar:
#include <algorithm> // contiene algortimos de búsqueda, partición, ordenación, etc
#include <array>
#include <cmath>      // Incluye funciones matemáticas como std::sqrt raiz cuadrada o std::pow potencia al cuadrado.
#include <cstdlib>    // Contiene las macros EXIST_SUCCESS y EXIT_FAILURE.
#include <iostream>   // Para imprimir con cout punteros por ejemplo
#include <filesystem> // libreria para manejo de archivos
#include <fstream>    // flujos i/o a ficheros
#include <print>      // contiene las funciones std::print y std::println (incluye salto de línea)
#include <ranges>     // funciones para trabajar con rangos, algo iteradod e princpio a fin,
// contiene vistas, visita por conjuntos
#include <random>    // Generador de radoms
#include <string>    // cadena de caracteres estándar
#include <vector>    // contenedor secuencial (contiguos en memoria vistual) recomendado por defecto en C++
#include <generator> // secuencia de elementos a demanda de una corrutina

// biblioteca no-estádar:
#include <nlohmann/json.hpp>

struct Point
{
    double x;
    double y;
};

struct Character
{
    std::string name; // nombre
    std::string town; // población
    Point location;   // posición en un mapa 2-dimensional
    bool leader;      // ¿es líder?
};

void from_json(nlohmann::json const &j, Character &c)
{
    j.at("name").get_to(c.name);
    j.at("town").get_to(c.town);
    // obtenemos el array de dos doubles de clave "location":
    auto const coordinates = j.at("location").get<std::array<double, 2>>();
    // cargamos los doubles en c.location:
    c.location.x = coordinates[0]; // primera entrada del array como abscisa
    c.location.y = coordinates[1]; // segunda entrada del array como ordenada
    j.at("leader").get_to(c.leader);
}

// Corrutina | Generador
template <typename T>
auto get_from_jsonl(std::filesystem::path pth) -> std::generator<T>
{
    if (pth.extension() != ".jsonl")
    {
        throw std::ios::failure{"the extension of the JSON Lines file must be'.jsonl'"};
    }
    
    auto ifs = std::ifstream{pth, std::ios::binary};
    if (!ifs)
    {
        throw std::ios::failure{"unable to open the JSON Lines file"};
    }
    auto ln = std::string{};
    while (std::getline(ifs, ln))
    {
        co_yield nlohmann::json::parse(ln).get<T>();
    }

}

// - - - - - - - - - - - - - - - - - - - - - - - -

auto same_town(Character character_a, Character character_b) -> bool
{
    return character_a.town == character_b.town;
}

// - - - - - - - - - - - - - - - - - - - - - - - -

auto main() -> int
{
    // Alias
    namespace stdr = std::ranges;
    namespace stdv = std::views;

    // 2. Vector de zombies
    auto gen = std::mt19937{std::random_device{}()};
    auto distr_x = std::uniform_real_distribution{100.0, 1400.0};
    auto rand_x = [&]() -> double {
        return distr_x(gen);
    };
    auto distr_y = std::uniform_real_distribution{150.0, 1100.0};
    auto rand_y = [&]() -> double {
        return distr_y(gen);
    };

    auto vector_zombies = std::vector<Point>{};
    for (auto i = int{0}; i < 300; i++)
    {
        auto const location_zombie = Point{.x = rand_x(), .y = rand_y()};
        vector_zombies.push_back(location_zombie);
    }

    // Lamda calcula distancia.
    auto distance = [](Point const &p1, Point const &p2) -> double
    {
        return std::sqrt(std::pow((p1.x - p2.x), 2) + std::pow((p1.y - p2.y), 2));
    };

    // 1. Vector de personajes.
    auto vector_characters = std::vector<Character>{};
    for (Character character : get_from_jsonl<Character>("../../walking_dead.jsonl"))
    {
        vector_characters.push_back(character);
    }

    stdr::sort(vector_characters, {}, &Character::town);

    for (auto const &characters_chunked_by_town : vector_characters | stdv::chunk_by(same_town))
    {
        auto town = stdr::begin(characters_chunked_by_town)->town;

        std::println("_________________{}_________________", town);

        stdr::sort(characters_chunked_by_town, {}, &Character::name);

        int zombies_next_to_leader = 0;
        for (auto const &character : characters_chunked_by_town)
        {
            std::string is_leader = character.leader ? "[[Leader]]" : "";
            std::println("{:>8} | x: {:>5}, y {:>5} {}", character.name, character.location.x, character.location.y, is_leader);

            if (character.leader)
            {
                // 3. Calcular Zombies a una distancia inferior del leader de 250m.
                for (auto const &zombie_location : vector_zombies)
                {
                    auto distance_to_leader = distance(character.location, zombie_location);
                    if (distance_to_leader < 250.0) {
                        zombies_next_to_leader++;
                    }
                }
            }
        }

        std::println("Population: {}", characters_chunked_by_town.size() * 10);
        std::println("Number of dangerously close walkers: {}", zombies_next_to_leader);
        std::println("");
    }

    return EXIT_SUCCESS;
}