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
// #include <string_view>
// #include <functional>

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

// auto es_igual = std::ranges::equal_to{};

// Nota función main:
// Si no se especifica ningún valor devuelto en main,
// el compilador proporciona un valor devuelto de cero.
// int main(int argc, char *argv[]);
// argc: Un entero que contiene el número de argumentos que aparecen detrás de argv. El parámetro argc es siempre mayor o igual que 1.
// argv[0] es el comando con el que se invoca el programa.
//  - argv[1] es el primer argumento de la línea de comandos.
//  - El último argumento de la línea de comandos es argv[argc - 1]
//  - y argv[argc] siempre es NULL
auto main() -> int
{
    // std espacio de nombres
    /*
     * Clases de datos, como std::string, std::vector y std::map.
     * Funciones de entrada/salida, como std::cin, std::cout y std::cerr.
     * Funciones de algoritmos, como std::sort(), std::find() y std::copy().
     * Funciones de tiempo y memoria, como std::time(), std::malloc() y std::free().
     */

    // 0. Alias
    namespace stdr = std::ranges;
    namespace stdv = std::views;

    // 1. Leer libreria JSON
    auto ruta_archivo_jsonl = "../../peter_parker.jsonl";
    // TODO check if is a path.
    auto flujo_de_archivo_de_entrada = std::ifstream{ruta_archivo_jsonl, std::ios::binary};

    if (flujo_de_archivo_de_entrada.fail())
    {
        std::print("No se ha podido cargar el fichero {}.", "peter_parker.jsonl");
        return EXIT_FAILURE;
    }

    // 2. Deserializar info archivo con la librería JSonNolan.
    auto objetivos_spiderman = std::vector<Mision>{};
    auto linea_a_parsear = std::string{};
    //   2.0 Abrir flujo del fichero Jsonl (std::ifstream{"path_to_file_builded", std::ios::binary}).
    while (std::getline(flujo_de_archivo_de_entrada, linea_a_parsear)) {
        //   2.a Parsear cada linea de Jsonl a un objeto (nlohmann::json::parse(linw).get<Object_struct()).
        auto objetivo_spiderman = nlohmann::json::parse(linea_a_parsear).get<Mision>();
        //   2.b Añadir cada objeto en un vector (.push_back(Element)).
        objetivos_spiderman.push_back(objetivo_spiderman);
    }

    //   2.F Cerrar flujo de datos (ifs.close()).
    flujo_de_archivo_de_entrada.close(); // cerramos el flujo al fichero

    // 3. Ordenar el vector (stdr::sort, namespace stdr = std::ranges).
    stdr::sort(objetivos_spiderman, {}, &Mision::district);

    for (auto objetivos_spiderman_chunk : objetivos_spiderman | stdv::chunk_by(es_mismo_distrito)) {
        auto nombres_vista_distrito = stdr::begin(objetivos_spiderman_chunk)->district;

        std::println("{}:", nombres_vista_distrito);

        // Ordenamos en un vector los enemigos.
        std::vector<std::string> enemigos_spiderman;
        for (Mision vista_mision : objetivos_spiderman_chunk) {
            enemigos_spiderman.push_back(vista_mision.type);
        }

        stdr::sort(enemigos_spiderman, {});

        std::unordered_map<std::string, int> conteo; // Unordered_map contiene elementos solo en forma de pares (clave-valor).
        
        for (const auto& nombre : enemigos_spiderman) {
            conteo[nombre]++;
        }

        for (const auto& par_enemigo_repeticion : conteo) {
            std::println("* {} -> {}", par_enemigo_repeticion.first, par_enemigo_repeticion.second);
        }
    }

    return EXIT_SUCCESS;
}