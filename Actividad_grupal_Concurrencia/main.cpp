#include <algorithm> // ordenar, buscar, hacer particiones... algoritmos de ordenacion
#include <chrono>
#include <cstdlib>   // Biblioteca Standard de C, contiene la función std::tolower (paso a minúscula)
#include <cstdint>   // for uint32_t
#include <fstream>   // flujos I/O a ficheros
#include <future>    // contiene la función std::async para la ejecución asíncrona de funciones
#include <iostream>
#include <limits>
#include <map>       // contenedor asociativo de clave única ordenada
#include <print>     // imprimir en consola/terminal
#include <random>
#include <ranges>    // vistas como chunk_by
#include <string>    // trabajar con string
#include <vector>    // contenedor secuencial de elementos contiguos. Amigo memoria cache.

//crear función para el tiempo

auto main() -> int
{


    /*
    using uf32 = std::uint_fast32_t; definimos el alias para std::uint_fast32_t

    
    */
    // std::vector<uint32_t> vector_enteros_uint_fast32_t = {}; // unsigned integer 32-bit type
    using uf32 = std::uint_fast32_t; //QUIQUEalias
    using clock = std::chrono::steady_clock; //QUIQUE alias reloj
    auto vector_enteros_uint_fast32_t = std::vector<uf32>{}; //QUIQUE aqui sería uint_fast32_t no el otro
    
    std::println("Tamaño del vector al inicio: {}", vector_enteros_uint_fast32_t.size());

    uf32 const min_value_uint_fast32_t = std::numeric_limits<uf32>::min();

    std::println("Mínimo valor uint_fast32_t: {}", min_value_uint_fast32_t);

    uf32 const max_value_uint_fast32_t = std::numeric_limits<uf32>::max();

    std::println("Máximo valor uint_fast32_t: {}", max_value_uint_fast32_t);

    // const int MAX_NUMBER_INTEGERS_CREATION = 100000000;
    const int MAX_NUMBER_INTEGERS_CREATION = 20;

    std::random_device random;  // a seed source for the random number engine
    std::mt19937 generator(random()); // mersenne_twister_engine seeded with rd()
    // std::uniform_int_distribution<> distribution(min_value_uint_fast32_t, max_value_uint_fast32_t);
    std::uniform_int_distribution<> distribution(uf32{0}, std::numeric_limits<uf32>::max()); // QUIQUE esto sería entre uf32{0} y std::numeric_limits<uf32>::max() (2^32 −1 =4,294,967,295)
    auto const start = clock::now();
    for (int i = 0; i < MAX_NUMBER_INTEGERS_CREATION; i++)
    {
        uf32 random_uint32_t_generated = distribution(generator);
        vector_enteros_uint_fast32_t.push_back(random_uint32_t_generated);

        std::println("Valor generado: {}", random_uint32_t_generated);
    }
     auto const end = clock::now();
    std::println("Tamaño del vector al rellenado: {}", vector_enteros_uint_fast32_t.size());

    std::println("Primer valor del vector: {}", vector_enteros_uint_fast32_t[0]);

    // Crear "chunks" del vector inicial (dividir), dependiendo del número de hilos.

    auto const numero_hilos_hardware = std::thread::hardware_concurrency();

    std::println("Números de hilos: {}", numero_hilos_hardware);
    std::println("duration: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()); //QUIQUE

    // Definimos el tamaño máximo del trozo

    auto const max_tamanio_trozo = vector_enteros_uint_fast32_t.size() / numero_hilos_hardware;

    std::println("Tamaño máximo del trozo/chunk: {}", max_tamanio_trozo);

    // uint32_t max_value = *std::max_element(vector_enteros_uint_fast32_t.begin(), vector_enteros_uint_fast32_t.end());
    // std::println("Tamaño máximo del trozo/chunk: {}", max_value);

    // Establecer funcion landa para hayar el máximo.
    auto capturar_maximo_valor = [] (auto chunk) -> uf32 // & Capturamos variable que vive fuera del ambito TODO Es necesario? QUIQUE  const auto & necesario no? para que no se ocpien los datos innecesariamente
    {
        auto result = std::max_element(chunk.begin(), chunk.end()); 
        return *result;
    }; // Las funciones Landa deben de estar cerradas.

    auto vector_max_valores_de_cada_trozo = std::vector<std::future<uf32>>{};

    std::println("Tamaño vector_max_valores_de_cada_trozo: {}", vector_max_valores_de_cada_trozo.size());

    std::println("- - - - - - - -");

    auto primer_trozo_temporal = vector_enteros_uint_fast32_t.begin(); //Es un iterador
    // std::println("Cominezo primer_trozo_temporal: {}", primer_trozo_temporal);
    auto ultimo_trozo_temporal = primer_trozo_temporal + max_tamanio_trozo; //QUIQUE se puede declarar dentro del bucle for?
    // std::println("Valor ultimo_trozo_temporal: {}", ultimo_trozo_temporal);

    // Hilos


    for (auto i = 0u; i < numero_hilos_hardware - 1; i++) // Debemos convertir el contador i en unsigned (sin signo). QUIQUE por qué? Por el enunciado?
    {
        vector_max_valores_de_cada_trozo.push_back(std::async(std::launch::async
                                                                , capturar_maximo_valor
                                                                , std::ranges::subrange{primer_trozo_temporal, ultimo_trozo_temporal}
                                                            )
                                                        );

        primer_trozo_temporal = ultimo_trozo_temporal;
        ultimo_trozo_temporal += max_tamanio_trozo;
    }
    // // hilo que se encarga del main
    capturar_maximo_valor(std::ranges::subrange{primer_trozo_temporal, vector_enteros_uint_fast32_t.end()});
    
    auto const start1 = clock::now();
    // Vector final con el máximo de cada hilo
    auto vector_max_valores_final = std::vector<uf32>{};
    for (std::future<uf32>& future : vector_max_valores_de_cada_trozo) { //QUIQUE agregar excepciones? try catch?
        uf32 capturado = future.get();
        std::println("TCapturado: {}", capturado);
        vector_max_valores_final.push_back(capturado);
    }
    auto const end1 = clock::now();
    std::println("duration: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count());
    std::println("- - - - - - - -");

    auto const start2 = clock::now();
    for (uf32 valor_final : vector_max_valores_final)
    {
        std::println("Valor final: {}", valor_final);
    }
    auto const end2 = clock::now();
    std::println("duration: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count());

    std::println("- - - - - - - -");

    auto maximo_valor = std::max_element(vector_max_valores_final.begin(), vector_max_valores_final.end()); 

    std::println("Máximo valor final: {}", *maximo_valor);

    return EXIT_SUCCESS;
}

