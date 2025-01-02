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
#include <boost/tokenizer.hpp> // Biblioteca externa boost

// Example object.
// {
//     "overall": 4.0,
//     "verified": false,
//     "reviewTime": "10 20, 2010",
//     "reviewerID": "A38NELQT98S4H8",
//     "asin": "0321719816",
//     "style": {
//         "Format:": " DVD-ROM"
//     },
//     "reviewerName": "WB Halper",
//     "reviewText": "I've been using Dreamweaver (and it's predecessor Macromedia's UltraDev) for many years.  For someone who is an experienced web designer, this course is a high-level review of the CS5 version of Dreamweaver, but it doesn't go into a great enough level of detail to find it very useful.\n\nOn the other hand, this is a great tool for someone who is a relative novice at web design.  It starts off with a basic overview of HTML and continues through the concepts necessary to build a modern web site.  Someone who goes through this course should exit with enough knowledge to create something that does what you want it do do...within reason.  Don't expect to go off and build an entire e-commerce system with only this class under your belt.\n\nIt's important to note that there's a long gap from site design to actual implementation.  This course teaches you how to implement a design.  The user interface and overall user experience is a different subject that isn't covered here...it's possible to do a great implementation of an absolutely abysmal design.  I speak from experience.  :)\n\nAs I said above, if you're a novice, a relative newcomer or just an experienced web designer who wants a refresher course, this is a good way to do it.",
//     "summary": "A solid overview of Dreamweaver CS5",
//     "unixReviewTime": 1287532800
// }

struct Producto_AMZ
{
    std::string asin; // Van a ser la keys.
    std::string reviewText; // VDebe ser tokenizado.
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Producto_AMZ, asin)

// Corrutina | Generador
template <typename T>
auto get_from_jsonl(std::filesystem::path pth) -> std::generator<T>
{
    // si el fichero no tiene la extensión correcta, lanzamos una excepción:
    if (pth.extension() != ".jsonl")
    {
        throw std::ios::failure{"the extension of the JSON Lines file must be'.jsonl'"};
    }
    // creamos un flujo de entrada al fichero JSON Lines:
    auto ifs = std::ifstream{pth, std::ios::binary};
    // si el flujo no está abierto, lanzamos una excepción:
    if (!ifs)
    {
        throw std::ios::failure{"unable to open the JSON Lines file"};
    }
    // deserializamos la información del fichero línea a línea:
    auto ln = std::string{};
    while (std::getline(ifs, ln))
    {
        // parseamos la línea y retornamos un valor de tipo T, suspendiendomomentáneamente la
        // ejecución de la corrutina:
        co_yield nlohmann::json::parse(ln).get<T>();
    }
} // el flujo al fichero es cerrado automáticamente por el destructor de std::ifstream

auto main() -> int
{
    namespace stdr = std::ranges;
    namespace stdv = std::views;

    auto vector_tokens_ains_productos_AMZ = std::vector<std::string>{};
    std::println("Tamaño vector de productos inicializado: {}", vector_tokens_ains_productos_AMZ.size());

    for (Producto_AMZ producto_AMZ : get_from_jsonl<Producto_AMZ>("../../amazon_software_5.jsonl"))
    {
        // ToDo comprobar que existe asin.
        vector_tokens_ains_productos_AMZ.push_back(producto_AMZ.asin);
    }

    std::println("Tamaño vector de productos rellenado: {}", vector_tokens_ains_productos_AMZ.size());
    std::println("Primer asin del primer producto: {}", vector_tokens_ains_productos_AMZ.at(1));

    // 4. Ordenar alfabéticamente el vector de tokens.
    stdr::sort(vector_tokens_ains_productos_AMZ);

    // 5. Preparamos un mapa para ser poblado.
    // Key: Clave la integer frecuencia de repetición.
    // Value: Array de string que tienen una frecuencia de ocurrencias.
    // Sabemos que el mapa esta ordenado de menor a mayor. Debemos indicar mayor stricto de orden.
    // pasamos stdr::greater como politica de ordenación a seguir (por defecto tenía less).
    auto freq_tokens_map = std::map<int, std::vector<std::string>, stdr::greater>{};

    // 6. Troceamos el vector, en subconjuntos por palabras idénticas.
    // Inicializamos la Lamda dentro del bucle foreach. Así su vida es valida solo durante el foreach.
    for (
        auto same_token = [](std::string const &t1, std::string const &t2)
        { return t1 == t2; };
        auto token_chunk : vector_tokens_ains_productos_AMZ | stdv::chunk_by(same_token))
    {
        // Contar repeticiones en cada trozo.
        // auto const freq = token_chunk.size();
        auto const freq = stdr::distance(token_chunk);
        auto const tkn = *stdr::begin(token_chunk); // para coger datos del iterador, con una copia, desreferenciar.
        // auto const tkn = stdr::begin(token_chunk)->parametro; // para coger datos del iterador, con su interfaz publica, desreferenciar.

        // Registrames pareja clave valor.
        freq_tokens_map[freq].push_back(tkn); // freq alias del sobrenombre que llama a la función pública de frecuancia.
    }

    // Imprimir 5 mayores frecuencias de ocurrencia en la terminal, acompañado de su listado de palabras.
    // Desectructuro a la pareja Destructure Binding"
    for (auto [freq, tokens] : freq_tokens_map | stdv::take(5))
    {
        std::print("{} --> ", freq);
        for (std::string tkn : tokens)
        {
            std::print("{}, ", tkn);
        }
        std::println();
    }

    return EXIT_SUCCESS;
}