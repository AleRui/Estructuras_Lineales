#include <algorithm> // ordenar, buscar, hacer particiones... algoritmos de ordenacion
#include <chrono>
#include <cstdlib>   // Biblioteca Standard de C, contiene la función std::tolower (paso a minúscula)
#include <cstdint>   // for uf32
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


auto main() -> int
{
    using uf32 = std::uint_fast32_t; //QUIQUE alias
    using clock = std::chrono::steady_clock; //QUIQUE alias reloj
    using namespace std::chrono_literals;

   
    
    auto vector_enteros_uint_fast32_t = std::vector<uf32>{};
    auto vector_enteros_no_thread_uint_fast32_t = std::vector<uf32>{};
    
    std::println("Tamaño del vector al inicio: {}", vector_enteros_uint_fast32_t.size());

    uf32 const min_value_uint_fast32 = std::numeric_limits<uf32>::min();

    std::println("Mínimo valor uint_fast32_t: {}", min_value_uint_fast32);

    uf32 const max_value_uint_fast32 = std::numeric_limits<uf32>::max();

    std::println("Máximo valor uint_fast32_t: {}", max_value_uint_fast32);

    // CONFIG
    // const int MAX_NUMBER_INTEGERS_CREATION = 100000000;
    const int MAX_NUMBER_INTEGERS_CREATION = 100000000;
    const int MAX_NUMBER_NO_THREAD= 100000000;
    std::random_device random;  // a seed source for the random number engine
    std::mt19937 generator(random()); // mersenne_twister_engine seeded with rd()
    // std::uniform_int_distribution<> distribution(min_value_uint_fast32, max_value_uint_fast32);
    // std::uniform_int_distribution<> distribution(uf32{0}, std::numeric_limits<uf32>::max());
    std::uniform_int_distribution<> distribution(uf32{0}, 1000000000);

    auto const clock_start_no_thread = clock::now();
    for (int i = 0; i < MAX_NUMBER_NO_THREAD; i++)
    {
        uf32 random_uf32_generated = distribution(generator);
        vector_enteros_no_thread_uint_fast32_t.push_back(random_uf32_generated);
    }
    
    auto maximo_valor_no_thread = std::max_element(vector_enteros_no_thread_uint_fast32_t.begin(), vector_enteros_no_thread_uint_fast32_t.end());
    std::println("Máximo valor final no thread: {}", *maximo_valor_no_thread);
    
    auto const clock_end_no_thread = clock::now();
        std::println("- - - - - - - -");

    std::println("Tiempo ejecución sin hilos: {}.", (std::chrono::duration_cast<std::chrono::seconds>(clock_end_no_thread-clock_start_no_thread).count()));
    auto const clock_start = clock::now();
    for (int i = 0; i < MAX_NUMBER_INTEGERS_CREATION; i++)
    {
        uf32 random_uf32_generated = distribution(generator);
        vector_enteros_uint_fast32_t.push_back(random_uf32_generated);
    }

    std::println("Tamaño del vector al rellenado: {}", vector_enteros_uint_fast32_t.size());

    std::println("Primer valor del vector: {}", vector_enteros_uint_fast32_t[0]);

    // Crear "chunks" del vector inicial (dividir), dependiendo del número de hilos.

    auto const numero_hilos_hardware = std::thread::hardware_concurrency();

    std::println("Números de hilos: {}", numero_hilos_hardware);

    // Definimos el tamaño máximo del trozo

    auto const max_tamanio_trozo = vector_enteros_uint_fast32_t.size() / numero_hilos_hardware;

    std::println("Tamaño máximo del trozo/chunk: {}", max_tamanio_trozo);


    auto vector_max_valores_de_cada_trozo = std::vector<std::future<uf32>>{};


    // Establecer funcion landa para hayar el máximo.
    auto capturar_maximo_valor = [] (auto chunk) -> uf32 // & Capturamos variable que vive fuera del ambito TODO Es necesario?
    // auto capturar_maximo_valor = [] (auto chunk) -> void // & Capturamos variable que vive fuera del ambito TODO Es necesario?
    {
        auto result = std::max_element(chunk.begin(), chunk.end());
        //std::println("max_value_result: {}", *result);
        return *result;
    }; // Las funciones Landa deben de estar cerradas.

    std::println("Tamaño vector_max_valores_de_cada_trozo: {}", vector_max_valores_de_cada_trozo.size());

    std::println("- - - - - - - -");

    auto iterador_primer_trozo_temporal = vector_enteros_uint_fast32_t.begin(); //Es un iterador
    auto iterador_ultimo_trozo_temporal = iterador_primer_trozo_temporal + max_tamanio_trozo; // Centinela

    // Hilos
    for (auto i = 0u; i < numero_hilos_hardware - 1; i++) // Debemos convertir el contador i en unsigned (sin signo).
    {
        //std::println("Hilo lanzado {}", i);
        vector_max_valores_de_cada_trozo.push_back(std::async(std::launch::async
                                                                , capturar_maximo_valor // funcion que se le pasa
                                                                , std::ranges::subrange{iterador_primer_trozo_temporal, iterador_ultimo_trozo_temporal} // parametro chunk que se le pasa a la funcion
                                                            )
                                                            // );
                                                    );

        iterador_primer_trozo_temporal = iterador_ultimo_trozo_temporal;
        iterador_ultimo_trozo_temporal += max_tamanio_trozo;
    }
    // hilo que se encarga del main
    //std::println("Hilo lanzado X pertenece al main");
    auto captura_valor_main = capturar_maximo_valor(std::ranges::subrange{iterador_primer_trozo_temporal, vector_enteros_uint_fast32_t.end()});
   // std::println("valor captura main {}", captura_valor_main);

    std::println("- - - - - - - -");

    // Vector final con el máximo de cada hilo
    auto vector_max_valores_final = std::vector<uf32>{};

    for (std::future<uf32>& future : vector_max_valores_de_cada_trozo) {
        uf32 capturado = future.get();
        //std::println("TCapturado: {}", capturado);
        vector_max_valores_final.push_back(capturado);
    }

    vector_max_valores_final.push_back(captura_valor_main);

    std::println("- - - - - - - -");

    for (uf32 valor_final : vector_max_valores_final)
    {
        std::println("Valor final: {}", valor_final);
    }

    std::println("- - - - - - - -");

    auto maximo_valor = std::max_element(vector_max_valores_final.begin(), vector_max_valores_final.end());

    std::println("Máximo valor final: {}", *maximo_valor);

    auto const clock_end = clock::now();

    std::println("- - - - - - - -");

    std::println("Tiempo ejecución con hilos: {}.", (std::chrono::duration_cast<std::chrono::seconds>(clock_end-clock_start).count()));

    return EXIT_SUCCESS;
}
