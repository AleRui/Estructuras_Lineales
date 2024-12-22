// ficheros de cabecera estándar:
#include <algorithm>  // contiene algortimos de búsqueda, partición, ordenación, etc
#include <iostream>   // Para imprimir con cout punteros por ejemplo
#include <filesystem> // libreria para manejo de archivos
#include <fstream>    // flujos i/o a ficheros
#include <print>      // contiene las funciones std::print y std::println (incluye salto de línea)
#include <ranges>     // funciones para trabajar con rangos, algo iteradod e princpio a fin,
                      // contiene vistas, visita por conjuntos
#include <string>     // cadena de caracteres estándar
#include <vector>     // contenedor secuencial (contiguos en memoria vistual) recomendado por defecto en C++
#include <generator>  // secuencia de elementos a demanda de una corrutina

// biblioteca no-estádar:
#include <nlohmann/json.hpp>
// .hpp header file fichero de cabecera estrictamente de C++, no funciona en C.


struct Target {
    std::string name;
    bool achieved;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Target, name, achieved)

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> // Familia uniparamétrica de funciones.
auto get_from_jsonl(std::filesystem::path pth) -> std::generator<T>
{
    if(pth.extension() != ".jsonl") {
        throw std::ios::failure{"Unable to open JSON LINES file."};
    }

    auto res = std::vector<Target>{};

    auto ifs = std::ifstream{pth, std::ios::binary};

    if (!ifs) {
        std::print("incapaz de abrir el fichero JSON Lines");
        throw std::ios::failure{"The extension of the Json Lines file must b jsonl."};
    }

    auto ln = std::string{};

    while (std::getline(ifs, ln)) {
        auto trgt = nlohmann::json::parse(ln).get<T>();
        co_yield trgt;
    }
}


auto main() -> int
try {
    // alias para espacios de nombres:
    namespace stdr = std::ranges; // creo un alias para este espacio de nombres.
    namespace stdv = std::views;

    // auto targets = std::vector<Target>{};

    // // Generador de Targets. Con corrutina.
    // for (Target trgt : get_from_jsonl<Target>("../../military_camp.jsonl")){
    //     targets.push_back(trgt);
    // }
    auto targets = get_from_jsonl<Target>("../../military_camp.jsonl") | stdr::to<std::vector>();

    stdr::sort(targets, {}, &Target::name);

    for (auto target_chunk : targets
                        | stdv::chunk_by( [](Target const& t1, Target const& t2) { return t1.name == t2.name; })) {
         
        auto const name = stdr::begin(target_chunk)->name;

        auto const achvd = stdr::count_if(target_chunk, &Target::achieved);

        auto const total = stdr::distance(target_chunk);

        std::println("{:>15} --> {}/{}", name, achvd, total);
    }

    return EXIT_SUCCESS;
}
catch (std::exception& e) {
    std::print("{}", e.what());
    return EXIT_FAILURE;
}
catch (...) {
    std::print("Unexpected exception.");
    return EXIT_FAILURE;
}