// ficheros de cabecera estándar:
#include <algorithm> // contiene algortimos de búsqueda, partición, ordenación, etc
#include <iostream>  // Para imprimir con cout punteros por ejemplo
#include <fstream>   // flujos i/o a ficheros
#include <print>     // contiene las funciones std::print y std::println (incluye salto de línea)
#include <ranges>    // funciones para trabajar con rangos, algo iteradod e princpio a fin,
                     // contiene vistas, visita por conjuntos
#include <string>    // cadena de caracteres estándar
#include <vector>    // contenedor secuencial (contiguos en memoria vistual) recomendado por defecto en C++

// biblioteca no-estádar:
#include <nlohmann/json.hpp>
// .hpp header file fichero de cabecera estrictamente de C++, no funciona en C.
// #include <json-develop/include/nlohmann/json.hpp>

/*
// class Target {
//     std::string name;
//     bool achieved;

//     // ... crear GETs SETs
// }

// Cambio class por struct.
// En una clase es todo privado por defecto.
// En un struct todo es público por defecto.
// información relevante de un objetivo: (es un agregado de datos)
struct Target {
    std::string name; // tipo de objetivo
    bool achieved;    // ¿está el target completado?   
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Target, name, achieved) // Macro. Se deben evitar. Informo a la biblioteca de la clase
// y en el orden.
// Los nombres deben coincidir con la keys del JSon.

////////////////////////////////////////////////////////////////////////////////////////////////////

auto same_name(Target t1, Target t2) -> bool // Predicado es que se devuelve verdadero o falso es BINARIO
{
    return t1.name == t2.name;
}
*/
// void incremento(double v) // paso por valor: le pasa una copia del valor que se le pasa

// void incremento(double& v) // paso por referencia: le pasa una copia del valor que se le pasa
// { // void funcion no devuelve nada
//     v += 1.2;
// }

void imprimir_mensaje(std::string const& str) // & la referencia en este caso es lógico y necesario
{                                             // la función no está mutando, solo lo lee
    // str += "PEL";  // No lo permite        // añadimos const para pedir que le objeto no va a cambiar
    std::println("{}", str);                  
}

auto main() -> int
{
    // auto d = double{7.8};
    // auto& r = d; // introducimos una referncia al double d, es decir un alias para dicho objeto
    // r += 1.2;
    // std::println("d = {}, r = {}", d, r);
    // incremento(d);
    // std::println("d = {}", d);

    // auto i = int{1};
    // auto const i = int{1}; // lo convierte en un objeto que no puede cambiar
    // i += 2;
    // std::println("i = {}", i);

    // auto msg = std::string{"Hello world!"};
    // auto const msg = std::string{"Hello world!"};
    // imprimir_mensaje(msg);

    // msg += " PEL"; // Esto fallaría si msg es constante
    // imprimir_mensaje(msg);

    auto n = int{0}; // ocupa 4 bytes
    // Buscar dirección de memoria vistual de esta variable
    // y quiero guardar esa dirección en memoria en otra variable
    int* p1 = &n; // & es un operador unario para obtener la dirección del entero "n"
                 // int* p es un puntero, dirección en memoria de un objeto
    auto d = double {9.9}; // ocupa 8 bytes
    double* p2 = &d;             // Un puntero en memoria ocupa
    // std::println no puede imprimir puntero como tales.
    std::cout << p1 << '\n';
    std::cout << p2 << '\n';

    // cuanto ocupa en memoria p1 y p2
    // todos ocupan 8 bytes porque almacenan direcciones de memoria
    std::println("{} bytes, {} bytes", sizeof(p1), sizeof(p2));
}

/*
auto main() -> int
{

    // auto t = Target{"soldier", true};
    // auto t = Target{.name = "soldier", .achieved = true}; // .name y .achieved son inicializadores
    // std::println("nombre: {}, completado; {}", t.name, t.achieved);

    // alias para espacios de nombres:
    namespace stdr = std::ranges; // creo un alias para este espacio de nombres.
    namespace stdv = std::views;

    auto targets = std::vector<Target>{}; // vector de objetos Target inicialmente vacío. Ya tiene dirección de memoria pero no tiene targets.
    // Necesitamos un contenedor capaz de crecer en un tiempo de ejecución.
    std::println("{}", targets.size()); // imprime 0 porque el vector esta inicializado pero vacio.
    std::println("\n");
    
    // creamos un flujo de entrada al fichero JSON Lines:
    auto ifs = std::ifstream{"../../military_camp.jsonl", std::ios::binary}; // ifs in file stream, flujo a fichero de entrada argumentos: path, 
    // asciendo dos niveles en mi arbold e directorio porque es donde esta el .exe de mi programa.
    // std::ios::binary garantiza la compatibilidad entre SO, con saltos de entrada...

    if (!ifs) { // si el flujo no se ha producido
        std::print("incapaz de abrir el fichero JSON Lines");
        return EXIT_FAILURE;
    }

    // deserializamos la información del fichero para registrarla en el vector:
    auto ln = std::string{}; // string auxiliar donde almacenar las líneas del fichero JSON Lines
    while (std::getline(ifs, ln)) { //  getlines devuelve true si es capaz de extraer la linea
        std::println("{}", ln);
        // TODO Deberíamos poder capturar la excepción.
        auto trgt = nlohmann::json::parse(ln).get<Target>(); // parseamos la línea y obtenemos un Target
        targets.push_back(trgt); // enviamos una copia del Target al fondo del vector (que es un contenedor).
        // con push_back cada vez que en cuentre un objeto target lo nañado al fondo del vector.
    }// cuando getline devuelva false cortará elñ bucle.

    std::println("\n");

    ifs.close(); // cerramos el flujo al fichero

    for (Target t : targets) {
        std::println("tipo: {}, conseguido: {}", t.name, t.achieved);
    }

    std::println("\n");

    // ordenamos el vector de Targets por orden lexicográfico de "name":
    // garantizo que todos los objetos del mismo nombre estánb agrupados.
    // std::ranges::sort(targets,std::ranges::less{}, &Target::name);
    stdr::sort(targets, {}, &Target::name); // las dos llaves es porque asumimos que la politica de ordenación es de menor a mayory no hace falta configurarla
    
    for (Target t : targets) {
        std::println("tipo: {}, conseguido: {}", t.name, t.achieved);
    }

    std::println("\n");

    // iteramos los subgrupos del vector de targets de idéntico nombre, imprimiendo
    // el nombre del primer representante de cada subgrupo:
    // for (auto target_chunk : targets | std::views:chunk_by(same_name)) { // chunk_by agrupar por vista
    // | es un pipe, una tubería
    for (auto target_chunk : targets | stdv::chunk_by(same_name)) { // chunk_by agrupar por vista
        auto nm = stdr::begin(target_chunk)->name; // ve a la primera casilla del trozo del vector que has visto y coge lo que es
        
        // Contador para sumar el número de elementos de cada nombre.
        // auto achvd = int{0};
        // for (Target t : target_chunk) {
        //     if (t.achieved) {
        //         ++achvd;
        //     }
        // }
        auto achvd = stdr::count_if(target_chunk, &Target::achieved);

        // Contador para sacar los totales por nombre
        // auto total = int{0};
        // for (Target t : target_chunk) {
        //     ++total;
        // }
        auto total = stdr::distance(target_chunk);

        // Popdemos usar un algoritmo de una biblioteca estandar para
        // recorrer un bucle y si se cumple la condición que se vaya obteniendo el
        // println align :> izq :< der :^center
        // std::println("tipo: {} --> {}/{}", nm, achvd, total);
        std::println("{:>15} --> {}/{}", nm, achvd, total);
    }

    return EXIT_SUCCESS;
}
*/