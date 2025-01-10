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

struct Producto_AMZ
{
    std::string asin;       // Van a ser la keys.
    std::string reviewText; // Debe ser tokenizado.
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Producto_AMZ, asin, reviewText)

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
        // parseamos la línea y retornamos un valor de tipo T, suspendiendo momentáneamente la
        // ejecución de la corrutina:
        // co_yield nlohmann::json::parse(ln).get<T>();
        auto const ln_object_nlohmann_parsed = nlohmann::json::parse(ln); // ln es un std::string conteniendo una línea JSON del fichero
        auto review = std::string{};
        try
        {
            review = ln_object_nlohmann_parsed.at("reviewText");
            if (review.size())
            {
                co_yield ln_object_nlohmann_parsed.get<T>();
            }
        }
        catch (nlohmann::json::out_of_range const &e)
        {
            // si la clave "reviewText" no existiera, ignoramos la línea JSON (por ejemplo continuando un bucle for/while)
        }
    }
} // el flujo al fichero es cerrado automáticamente por el destructor de std::ifstream

// --------------------------------------------------------------------------------------

auto main() -> int
{
    namespace stdr = std::ranges;
    namespace stdv = std::views;

    // 1. Crear mapa de productos.
    auto map_ains_reviews = std::map<std::string, std::vector<std::ptrdiff_t>>{};
    for (Producto_AMZ item_producto_AMZ : get_from_jsonl<Producto_AMZ>("../../amazon_software_5.jsonl"))
    {
        // 1.b Tokenizar el reviewText
        auto tokens = stdr::to<std::vector<std::string>>(boost::tokenizer{item_producto_AMZ.reviewText});
        std::ptrdiff_t tokenized_reviews_length = tokens.size();
        map_ains_reviews[item_producto_AMZ.asin].push_back(tokenized_reviews_length);
    }

    // 2. Crear segundo Mapa con key referencia asin y extensión medias (double) de sus reviews de opinión.
    auto map_asin_averages = std::map<std::string, double>{};
    auto vector_averages = std::vector<double>{};

    for (auto [asin, tokenized_reviewTexts] : map_ains_reviews)
    {

        double total_tokenized_in_reviews = 0;
        int num_tokenized_riviews = 0;

        for (const auto &token_review : tokenized_reviewTexts)
        {
            total_tokenized_in_reviews += token_review;
            num_tokenized_riviews++;
        }
        map_asin_averages[asin] = total_tokenized_in_reviews / num_tokenized_riviews;
        vector_averages.push_back(total_tokenized_in_reviews / num_tokenized_riviews);
    }

    std::println("* 5 primeros productos registrados en el mapa junto a sus medias asociadas.");
    for (auto [asin, average] : map_asin_averages | stdv::take(5))
    {
        std::println("asin: {} --> media: {} ", asin, average);
    }

    std::println("-----");

    // 3. Las 5 tokenized_reviews_lengthes averages más altas.
    std::sort(vector_averages.begin(), vector_averages.end(), std::greater<>());

    auto map_top_five_averages_referencias = std::map<double, std::vector<std::string>, stdr::greater>{};
    for (auto average_in_top_five_ranking : vector_averages | stdv::take(5))
    {
        for (auto [asin, average] : map_asin_averages)
        {
            if (average_in_top_five_ranking == average)
            {
                map_top_five_averages_referencias[average_in_top_five_ranking].push_back(asin);
            }
        }
    }

    std::println("* Top 5 tokenized_reviews_lengthes medias de textos de opinión y su valor asociado un listado de los productos.");
    for (auto [average, asins] : map_top_five_averages_referencias)
    {
        std::print("Media: {} --> ", average);
        for (std::string asin : asins)
        {
            std::print("{}, ", asin);
        }
        std::println();
    }

    return EXIT_SUCCESS;
}