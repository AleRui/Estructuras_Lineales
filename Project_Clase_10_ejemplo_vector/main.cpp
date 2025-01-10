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
   T* v_; // Apunta al inicio del vector en el free store
   T* space_; // Apunta a uno más del último almacenado. del vector en el free store.
   T* last_; // Apunta a un hipotético elemento pasado 1 de los reservados (centinela).
   // Los guines bajos sufijos se usan variables privadas de una clase. Standrd en compañías de programación.

   // por defecto todo parametro/metodo es privado.
   // debo usar public para para declarar lo que va a ser público.

   // Esto punteros apuntan al free store al array de datos.

   // Da igual el tipo y la cantidad que contenga que el vector en la pila siempre ocupara lo mismo.
   public:
      // Vector() // El constructor debe de llamarse igual que la clase a la que pertence.
      // { // Cuerpo del constructor. Por defecto construyen un vector vacio.
      //    //Inicialiciar las condiciones.
      //    v_ = new T[0]; // Creo un array de 0 elementos. El new retorna un dirección en memŕia que es válida.
      //    // Ahora space_ y last_ apuntan a la misma direcicón de memoria.
      //    space_ = v_ ;
      //    last_ = v_ ;
      // }
      Vector()
         : v_{new T[0]}, space_{v_}, last_{v_} // Ideomaticamente y por rendimiento se inicia de esta manera el constructor.
      {}

      // Ténica RAII.
      // Creamos un destructor (solo puede haber uno).
      ~Vector() // destructor que destruyue los elementos contenido y libera la memoria.
      {
         delete[] v_; // [] Porque hay que indicar que se quiere destruir tood un array que e,pieza en v_.
      }

      // --------------------------------------
      // Funcioón de capacidad || Aritmética de punteros. Mirar Lectura.
      // size() Tamaño del Vector
      auto size() -> std::size_t { return space_ - v_ ; } // Indica el tamaño de contenedores de datos.
      // Capacidad
      auto capacity() -> std::size_t { return last_ - v_ ; } // Número de casillas que sestá ocupado y lo que no.
      // Empty si tiene o no casillas reservadas
      // auto empty() -> bool { return size() == 0; }
      auto empty() -> bool { return v_ == space_ ; }

      // --------------------------------------
      // Funciones de acceso.
      // Sobrecarga de operadores operator[].
      // Da acceso de escritura y lectura a ese hueco del array.
      operator[](std::size_t idx) -> T& { return v_[idx]; } // Devuelvo un acceso de lectur al original T&.

};

auto main() -> int
{
   // auto words = std::vector<std::string>{}; // Vector de palabras inicialmente vacio.
   //auto words = Vector<std::string>{}; // Vector de palabras inicialmente vacio.
   auto words = Vector<double>{};

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