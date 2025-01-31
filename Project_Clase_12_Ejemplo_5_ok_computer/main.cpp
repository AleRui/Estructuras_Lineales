#include <cstdlib>    // contiene EXIT_SUCCESS y EXIT_FAILURE
#include <algorithm>  // algoritmos de partición, ordenación, etc
#include <functional> // contiene la función std::not_fn para negar lambdas
#include <iostream>   // contiene std::cin (flujo de entrada estándar)
#include <print>
#include <ranges>
#include <string>
#include <vector>    // Contenedor, ayuda a la cache.

namespace stdr = std::ranges;
namespace stdv = std::views;

struct Album {
   std::string title; // título
   std::string band;  // banda
   std::string genre; // género
};

auto main() -> int
{
   // No declaramos el vector constante porque lo vamos a mutar.
   auto albums = std::vector<Album>{ // No es buena idea dejarlo como variable global. Lo metemos en el main.
      {.title = "the dark side of the moon", .band = "pink floyd", .genre = "rock"},
      {"tubular bells", "mike oldfield", "new age"},
      {"ok computer", "radiohead", "rock"},
      {"the magnificent tree", "hooverphonic", "pop"},
      {"favourite worst nightmare", "arctic monkeys", "rock"},
      {"the four seasons", "vivaldi", "classical"},
      {"parachutes", "coldplay", "pop"},
      {"the suburbs", "arcade fire", "rock"},
      {"gran turismo", "the cardigans", "pop"}
   };

   // solicitamos un título de álbum al usuario:
   std::print("Introduce un título de álbum: ");
   auto band_to_search = std::string{};
   std::getline(std::cin, band_to_search); // Introduce TERMINAL

   if (
      // búsqueda lineal por título en el vector:
      // ña función find devuelve un iterador que solo se usa en el ambito de la funcion.
      auto const search_iter = stdr::find(albums, band_to_search, &Album::title);
      // - busca en el vector
      // - lo buscado
      // - devuelve una proyección, cual de lso tres string estoy buscando.
      // Condición
      search_iter != albums.end() // el álbum se encuentra en la discografía
   ) {
      auto const& found_genre = search_iter->genre; // género del álbum solicitado por el usuario

      // predicado unario que emplear en las particiones del vector:
      // nota: [&] permite a la lambda capturar por referencia variables externas a ella
      auto same_genre = [&](Album const& a) -> bool { return a.genre == found_genre; };

      // partición estable a la derecha del álbum solicitado, situando al frente
      // del subrango [search_iter + 1, albums.end()) todos los discos de género
      // idéntico al solicitado; [part_iter_1, part_iter_2) contiene los títulos que quedan
      // al fondo del subrango que no son del mismo género:
      // serach_iter + 1, porque ya hemos encontrado el disco, a la derecha en el vector.
      // albums.end() porque queremos que llegue hasta el final.
      // same_genre es el predicado que se va a aplicar.La lambda comprueba los que son el mismo que el seleccionado.
      // se devbuelve una tupla con los dos iteradores. Los que no cumplieron la condición. Al primero que no, y al último que no que coincidira con el end().
      // Podríamos poner un guion bajo si no vamos a ustilizar ese elemento.
      // auto [part_iter_1, _] = stdr::stable_partition(search_iter + 1, albums.end(), same_genre);
      auto [part_iter_1, part_iter_2] = stdr::stable_partition(search_iter + 1, albums.end(), same_genre);

      // partición estable a la izquierda del álbum solicitado, situando al frente
      // del subrango [albums.begin(), search_iter) todos los discos de género
      // distinto al solicitado; [part_iter_3, part_iter_4) contiene los títulos que quedan
      // al fondo del subrango que sí son del mismo género:
      // albums.begin() marca el inciio del subrango.
      // search_iter marca el final del subrango.
      // same_genre es el predicado que se va a aplicar.La lambda comprueba los que no son el mismo que el seleccionado.
      auto [part_iter_3, part_iter_4] = stdr::stable_partition(albums.begin(), search_iter, std::not_fn(same_genre));

      std::println("{:_^60}", "");

      // subrango situado en el extremo izquierdo del vector que contiene
      // títulos de género distinto al solicitado:
      // nota: {:>25} genera un hueco vacío de 25 caracteres de longitud, introduciendo el string
      //       a.title justificado a la derecha; {:<15} justifica a.band a la izquierda en un hueco
      //       de 15 caracteres de longitud
      for (Album const& a : std::ranges::subrange{albums.begin(), part_iter_3}) {
         std::println("{:>25} by {:<15} | {}", a.title, a.band, a.genre);
      }
      std::println("{:_^60}", "");

      // subrango de títulos de igual género al solicitado:
      for (Album const& a : std::ranges::subrange{part_iter_3, part_iter_1}) {
         std::println("{:>25} by {:<15} | {}", a.title, a.band, a.genre);
      }
      std::println("{:_^60}", "");
      
      // subrando situado en el extremo derecho del vector que contiene
      // títulos de género distinto al solicitado
      for (Album const& a : std::ranges::subrange{part_iter_1, albums.end()}) {
         std::println("{:>25} by {:<15} | {}", a.title, a.band, a.genre);
      }
      std::println("{:_^60}", "");
   }
   else {
      std::println("Lo lamento, '{}' no está en la discografía", band_to_search);
   }

   return EXIT_SUCCESS;
}