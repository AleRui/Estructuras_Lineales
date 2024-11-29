// ficheros de cabecera estándar:
#include <algorithm> // contiene algortimos de búsqueda, partición, ordenación, etc
#include <iostream>  // Para imprimir con cout punteros por ejemplo
#include <fstream>   // flujos i/o a ficheros
#include <print>     // contiene las funciones std::print y std::println (incluye salto de línea)
#include <ranges>    // funciones para trabajar con rangos, algo iteradod e princpio a fin,
                     // contiene vistas, visita por conjuntos
#include <string>    // cadena de caracteres estándar
#include <vector>    // contenedor secuencial (contiguos en memoria vistual) recomendado por defecto en C++

#include <typeinfo>  // Library tipo de variable.
#include <generator> // secuencia de elementos a demanda de una corrutina

// biblioteca no-estádar:
#include <nlohmann/json.hpp>

struct Mision {
    std::string district;
    std::string type;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Mision, district, type) 

auto es_mismo_distrito(Mision distrito_a, Mision distrito_b) -> bool
{
    return distrito_a.district == distrito_b.district;
}

// Corrutina | Generador
auto get_from_jsonl(std::string file) -> std::generator<Mision>
{
    auto vector_mision = std::vector<Mision>{};

    auto ifs = std::ifstream{file, std::ios::binary};

    if (!ifs) {
        std::print("incapaz de abrir el fichero JSON Lines");
        throw std::ios::failure{"Unable open the Json Lines file."}; // Error de entrada salida
    }

    auto ln = std::string{};
    while (std::getline(ifs, ln)) {
        auto objetivo_spiderman = nlohmann::json::parse(ln).get<Mision>();
        co_yield objetivo_spiderman;
    }
}


auto main() -> int
{
    namespace stdr = std::ranges;
    namespace stdv = std::views;

    auto objetivos_spiderman = std::vector<Mision>{};

    for (Mision mision_spiderman : get_from_jsonl("../../peter_parker.jsonl")){
        objetivos_spiderman.push_back(mision_spiderman);
    }

    // auto objetivos_spiderman = get_from_jsonl("../../peter_parker.jsonl");

    stdr::sort(objetivos_spiderman, {}, &Mision::district);

    for (auto objetivos_spiderman_chunk : objetivos_spiderman | stdv::chunk_by(es_mismo_distrito)) {

        auto nombres_vista_distrito = stdr::begin(objetivos_spiderman_chunk)->district;

        std::println("{}:", nombres_vista_distrito);

        std::vector<std::string> enemigos_spiderman;
        for (Mision vista_mision : objetivos_spiderman_chunk) {
            enemigos_spiderman.push_back(vista_mision.type);
        }

        stdr::sort(enemigos_spiderman, {});

        // Unordered_map contiene elementos solo en forma de pares (clave-valor).
        std::unordered_map<std::string, int> contador;
        
        for (const auto& nombre : enemigos_spiderman) {
            contador[nombre]++;
        }

        for (const auto& par_enemigo_repeticion : contador) {
            std::println("*{:>20} -> {}", par_enemigo_repeticion.first, par_enemigo_repeticion.second);
        }

         std::println("\n");
    }

    return EXIT_SUCCESS;
}