// ficheros de cabecera estándar:
#include <algorithm> // contiene algortimos de búsqueda, partición, ordenación, etc
#include <cmath>     // Incluye funciones matemáticas como std::sqrt raiz cuadrada o std::pow potencia al cuadrado.
#include <cstdlib>   // Contiene las macros EXIST_SUCCESS y EXIT_FAILURE.
// #include <iostream>   // Para imprimir con cout punteros por ejemplo
// #include <filesystem> // libreria para manejo de archivos
// #include <fstream>    // flujos i/o a ficheros
#include <print>  // contiene las funciones std::print y std::println (incluye salto de línea)
//#include <ranges> // funciones para trabajar con rangos, algo iteradod e princpio a fin,
// contiene vistas, visita por conjuntos
#include <random> // Generador de radoms
#include <string> // cadena de caracteres estándar
#include <vector> // contenedor secuencial (contiguos en memoria vistual) recomendado por defecto en C++
// #include <generator>  // secuencia de elementos a demanda de una corrutina

struct Point
{
    double x;
    double y;
    double z;
};

struct Machine
{
    Point location;
    bool alert; // Si la distancia a Alou < 10.0
    bool attack; // Si la distancia a Aloy < 5.0
};

auto main() -> int
{
    // Alias
    namespace stdr = std::ranges;

    // distancia de Aloy (situada en el origen de coordenadas)
    // a un punto cualquiera del escenario

    // Expresión lamda.
    auto distance = [](Point const &p) -> double // const& captura por referencia, no hace copia, la captura y la trata en elambito de la funcion.
    {
        // descomposición (structured binding).
        auto const &[x, y, z] = p; // Descomposion, muy útil y conveniente.
        // auto x = p.x;
        // auto y = p.y;
        // auto z = p.z;
        // return std::sqrt( (x - 0.0)² + (y - 0.0)² + (z - 0.0)² );
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
    };

    // auto dist = double{distance(Point(-1.4, 4.5, 8.9))};

    auto gen = std::mt19937{std::random_device{}()}; // mersenne_twister_engine genera numeros aleatorio enteros sin signo. Entre 0 y 2^32 sin incluir el último.
    // Semillas aleatorios:
    // Hora actual
    // Pulsaciones y movientos de raton registrados por el pc.
    
    // Creamos distribución
    auto distr = std::uniform_real_distribution{-15.0, 15.0};

    // Lambda
    auto rand = [&] () -> double { // & utiliza por referencias, sin hacer copias usar variables de entornos de fuera.
        return distr(gen);
    };

    // auto d1 =  rand();
    // auto d2 =  rand();
    // auto d3 =  rand();

    // std::println("{}", d1);
    // std::println("{}", d2);
    // std::println("{}", d3);

    // Vector de máquinas incialmente vacio.
    auto machines = std::vector<Machine>{};
    machines.reserve(25); // ya conozco que el vector va a tener esta capacidad pues la reervo.

    for (auto i = int{0}; i < 25; i++)
    {
        // auto const loc = Point{rand(), rand(), rand()}; // localización arbitraria. Punto con 3 coordenadas random.
        auto const loc = Point{.x = rand(), .y = rand(), .z = rand()}; // .x, .y .z inicializadores desinados.
        // distance a Aloy
        auto const dis = distance(loc); // distancia de la localización generada a Aloy
        // Crea máquinas.
        // auto m = Machine{loc, dis < 10.0, dis < 5.0};
        auto const m = Machine{.location = loc, .alert = dis < 10.0, .attack = dis < 5.0};
        machines.push_back(m);
    }

    // Vamos a someter al vector a un algoritmo de particion.
    // Sabemos que todos los que están en ataque están en alerta.
    // todo los alemento que tiene al menos un true se van al frente, el booleano alerta es verdadero.
    auto [alert_it, last_1] = stdr::partition(machines, [](Machine const& m) -> bool { return m.alert; });
    // partition podria devolver dos cosas:
    // de donde a donde van los elementos que no cumplen la condición.
    // un iterador a la primera casilla una pasada la condición
    // y un iterador uno pasado el vector.
    // y capturamos la info con un structure binding

    // // Los que están fuera.
    // for (Machine const& m : stdr::subrange{alert_it, last_1})
    // {
    //     std::println("{}", distance(m.location));
    // }

    //
    auto [attack_it, last_2] = stdr::partition(stdr::subrange{machines.begin(), alert_it}, 
                                        [](Machine const& m) -> bool { return m.attack; }
        );
    
    // Los que si están en ataque.
    std::println("Ataque___________________");
    for (Machine const& m : stdr::subrange{machines.begin(), attack_it})
    {
        std::println("x: {:.1f}, y: {:.1f}, z: {:.1f} | ditance: {}", m.location.x, m.location.y, m.location.z, distance(m.location));
    }

    // Los que si están en alerta.
    std::println("Alerta___________________");
    for (Machine const& m : stdr::subrange{attack_it, alert_it})
    {
        std::println("x: {:.1f}, y: {:.1f}, z: {:.1f} | ditance: {}", m.location.x, m.location.y, m.location.z, distance(m.location));
    }

    return EXIT_SUCCESS;
}