#include <algorithm> // Incluye std::ranges::sort.
#include <cstdlib>   // Biblioteca Standard de C.
#include <iostream>  // Contiene función std::cin.
#include <print>     // imprimir en consola/terminal.
#include <string>    // trabajar con string.
#include <vector>    // contenedor secuencial homogéneo de elementos contiguos. Amigo memoria cache.

template<typename T> // T sera string, double, integer... Conforme se pida.
class Vector // Vamos a hacer un vector "de juguete".
{
   
   // vamos a crear 3 iteradores/punteros:
   // - v_ uno donde empieza la primera casilla del vector nuestro begin(). Del tipo de objeto que almacena el vector.
   // - space_ señala el prmer espacio reservado nuestro end();
   // - last_ uno pasado el último de los reservados (centinela)
   T* v_;
   T* space_;
   T* last_;
   // Los guines bajos sufijos se usan variables privadas de una clase. Standrd en compañías de programación.

      // por defecto todo parametro/metodo es privado.
   // debo usar public para para declarar lo que va a ser público.
   public:

};

auto main() -> int
{
   auto words = std::vector<std::string>{}; // Vector de palabras inicialmente vacio.
   auto words = Vector<std::string>{}; // Vector de palabras inicialmente vacio.

   auto wrd = std::string{};

   // Mientra que no se haga CTRL+Z o CTRL+D, introduce palabras.
   auto mssg = [] () {
      std::print("Introduce una palabra: ");
      return true;
   };

   while (mssg() and std::getline(std::cin, wrd))
   {
      words.push_back(wrd);
      // std::print("Introduce una palabra: ");
   }
   
   std::ranges::sort(words);

   for (std::string  const& w : words)
   // for (auto it = words.begin(); it != words.end(); ++it)
   {
      std::println("{}", w);
      // std::println("{}", *it);
   }

   return EXIT_SUCCESS;
}