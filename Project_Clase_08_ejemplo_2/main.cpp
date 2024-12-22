#include <algorithm> // contiene algoritmos de busqueda, partición ordenación
#include <cstdlib>   // contiene las macros EXIT_SUCCESS y EXIT_FAILURE
#include <iostream>  // contiene los objetos globales std::cin y std::cout
#include <map>       // contenedor asociativo de clave única ordenada
#include <print>
#include <string>
#include <vector>    // contenedor secuencial recomendado por defecto

auto main() -> int
{
    // Mapa clave nombre contacto, valor num telf.
    // Las claves no se pueden repetir, sino se sobreescribiría el valor.
    // Multimaps permite claves repetidas.
    // auto phone_book = std::map<std::string, std::string>{}; // iniciamos vacío.
    // auto phone_book_desc = std::map<std::string, std::string, std::ranges::greater>{}; // cambio política de mayor a menor.

    // phone_book["Javier"] = "612345678";
    // phone_book["Maria"] = "698765432";
    // phone_book["Alberto"] = "611222333";

    // Structure binding, 
    // for (auto const& [k, v] : phone_book) { // const& referencias string constantes.
    //     std::println("{} --> {}", k, v);
    // }


    // clave: nombre del grupo (string); valor asociado: integrantes del grupo (vector de string)
    auto bands = std::map<std::string, std::vector<std::string>>{}; // mapa inicialmente vacío

    bands["The Beatles"] = std::vector<std::string>{"Paul McCartney",
                                                    "John Lennon",
                                                    "Ringo Starr",
                                                    "George Harrison"};

    bands["Radiohead"] = std::vector<std::string>{"Thom Yorke",
                                                  "Jonny Greenwood",
                                                  "Colin Greenwood",
                                                  "Ed O'Brien",
                                                  "Philip Selway"};

    // for (auto& [band, members] : bands) { // const& referencias string constantes.
    //     std::println("{}_____________", band);
    //     std::ranges::sort(members);
    //     for (std::string const& member : members) { 
    //         std::println(" * {}", member);
    //     }
    // }
    

    auto band_to_search = std::string{}; // inicialmente vacío
    std::print("Band to seach: "); // solicitamos al usuario la banda a buscar
    std::getline(std::cin, band_to_search); // registramos la banda en el string auxiliar desde la terminal
    // std::cin entrada standar, terminal

    // auto const it = bands.find(band_to_search); // id iterador Buscamos con find una flecha que pueda estar o no en el mapa y la devuelve.
    // if (bands.contains(band_to_search)) { // la banda solicitada está registrada como clave del mapa
    // if (it != bands.end()) { // La banda solicitada esta como clave del mapa.
    if (
        auto const it = bands.find(band_to_search); // Metemos dentro del if que es el ambito donde va a actuar la variable.
                                                    // La variable va a tener el tiempo de vida de la sentencia.
        it != bands.end()
    ) { // La banda solicitada esta como clave del mapa.
        auto& [_, members] = *it; // Structure Binding EL nombre lo ponemos con _ porque ya no nos interesa, y nos quedamos con members.
        // con & me garantizo que no hago una copia
        // [_, _] Con el Structure Binding preparo los valores de esta Tupla
        // con  * desreferencio el iterado it
        
        // std::ranges::sort(bands[band_to_search]); // ordenamos los integrantes de la banda
        //std::ranges::sort(bands[band_to_search]); // ordenamos los integrantes de la banda
        std::ranges::sort(members); // ordenamos los integrantes de la banda
        for (auto const& m : members) { // imprimimos a los integrantes de la banda
            std::println(" * {}", m);
        }
    }
    else { // la banda solicitada no está registrada en el mapa
        std::println("Sorry, '{}' is not on the list", band_to_search);
    }

    {
        // ... esto es un ambito que no tiene nombre. Pero tdo lo que hay aqui vive entre estas llaves.
    }

    // return EXIT_SUCCESS;
    return 0; // Es lo mismo
}