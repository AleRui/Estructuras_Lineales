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

    // #######################################################################
    //                               General
    // #######################################################################

    using uf32 = std::uint_fast32_t; //QUIQUE alias
    using clock = std::chrono::steady_clock; //QUIQUE alias reloj
    using namespace std::chrono_literals; // se necesita?

    // CONFIG
    const int MAX_NUMBER_INTEGERS_CREATION = 100000000;
    std::random_device random;  // Semilla aleatoria basada en el sistema o hardware
    std::mt19937 generator(random()); // Generador de números aleatorios Mersenne Twister usando la semilla
    std::uniform_int_distribution<> distribution(uf32{0}, MAX_NUMBER_INTEGERS_CREATION); // Distribución uniforme que generará números aleatorios

    // #######################################################################
    //                               Sin hilos
    // #######################################################################

    auto no_thread_fast32_t_int_vector = std::vector<uf32>{}; // Vector de enteros fast32_t

    std::println("---------------------------------");
    std::println("            Sin hilos            ");
    std::println("---------------------------------");

    auto const no_thread_clock_start = clock::now(); 

    // For para llenar el vector de randoms
    for (int i = 0; i < MAX_NUMBER_INTEGERS_CREATION; i++)
    {
        uf32 random_uf32_generated = distribution(generator);
        no_thread_fast32_t_int_vector.push_back(random_uf32_generated);
    }

    auto const no_thread_max_value = *std::max_element(no_thread_fast32_t_int_vector.begin(), no_thread_fast32_t_int_vector.end()); // Valor máximo del vector
    
    auto const no_thread_clock_end = clock::now();

    std::println("Valor máximo hallado sin hilos:   {}", no_thread_max_value);
    std::println("Tiempo ejecución sin hilos:       {} ms", (std::chrono::duration_cast<std::chrono::milliseconds>(no_thread_clock_end-no_thread_clock_start).count()));
    std::println("");

    // #######################################################################
    //                               Con hilos
    // #######################################################################
    
    auto fast32_t_int_vector = std::vector<uf32>{}; // Vector de enteros fast32_t
    uf32 const min_value_uint_fast32_t = std::numeric_limits<uf32>::min();
    uf32 const max_value_uint_fast32_t = std::numeric_limits<uf32>::max();
    
    std::println("---------------------------------");
    std::println("            Con hilos            ");
    std::println("---------------------------------");
    std::println("Tamaño del vector al inicio:      {}", fast32_t_int_vector.size());
    std::println("Mínimo valor uint_fast32_t:       {}", min_value_uint_fast32_t);
    std::println("Máximo valor uint_fast32_t:       {}", max_value_uint_fast32_t);
    std::println("");

    auto const clock_start = clock::now();
    
    // For para llenar el vector de randoms
    for (int i = 0; i < MAX_NUMBER_INTEGERS_CREATION; i++)
    {
        uf32 random_uf32_generated = distribution(generator);
        fast32_t_int_vector.push_back(random_uf32_generated);
    }

    auto const hardware_threads_count = std::thread::hardware_concurrency();  // Número de hilos disponibles en el hardware
    auto const max_chunk_size  = fast32_t_int_vector.size() / hardware_threads_count; // Definimos el tamaño máximo del trozo
    auto chunk_max_values_vector = std::vector<std::future<uf32>>{};  // Vector para almacenar los máximos valores de cada trozo

    // Establecer funcion lambda para hallar el máximo de cualquier chunk
    auto capture_max_value  = [] (auto const& chunk) -> uf32
    {
        return *std::max_element(chunk.begin(), chunk.end());
    };

    auto first_value_of_chunk = fast32_t_int_vector.begin(); // Primer valor del primer chunk, que luego se va actualizando
    auto last_value_of_chunk  = first_value_of_chunk + max_chunk_size; // Ultimo valor del primer chunk, que luego se va actualizando

    // For con los hilos, debemos convertir el contador i en unsigned (sin signo).
    for (auto i = 0u; i < hardware_threads_count - 1; ++i) // 
    {
        chunk_max_values_vector.push_back(std::async(std::launch::async // Debe ejecutarse en un hilo independiente
                , capture_max_value // Funcion que se le pasa
                , std::ranges::subrange{first_value_of_chunk, last_value_of_chunk} // Parametro chunk que se le pasa a la funcion
            )
        );

        first_value_of_chunk = last_value_of_chunk;
        last_value_of_chunk += max_chunk_size;
    }
    // Hilo que se encarga del main
    auto capture_main_value = capture_max_value(std::ranges::subrange{first_value_of_chunk, fast32_t_int_vector.end()});

    // Vector final con el máximo de cada hilo
    auto max_values_final_vector = std::vector<uf32>{};

    // Se añaden los valores de los hilos asíncronos al vector max_values_final_vector, que están en el vector chunk_max_values_vector
    for (std::future<uf32>& future : chunk_max_values_vector) {
        max_values_final_vector.push_back(future.get());
    }

    // Se añade el valor máximo del hilo main
    max_values_final_vector.push_back(capture_main_value);

    // Se obtiene el valor máximo final con la función std::ranges::max_element
    auto const final_max_value = *std::max_element(max_values_final_vector.begin(), max_values_final_vector.end());

    auto const clock_end = clock::now();

    std::println("Valor máximo hallado con hilos:   {}", final_max_value);
    std::println("Tiempo ejecución con hilos:       {} ms", (std::chrono::duration_cast<std::chrono::milliseconds>(clock_end-clock_start).count()));

    return EXIT_SUCCESS;
}
