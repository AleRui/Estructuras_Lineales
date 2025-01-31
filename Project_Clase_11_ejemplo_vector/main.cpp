#include <algorithm> // Incluye std::ranges::sort.
#include <cstdlib>   // Biblioteca Standard de C.
#include <iostream>  // Contiene función std::cin.
#include <exception> // Clase base de las que deriban todas las clases de excepción de C++. Tiene std::out_of_range
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
      auto capacity() -> std::size_t { return last_ - v_ ; } // Número de casillas que está ocupado y lo que no.
      // Empty si tiene o no casillas reservadas
      // auto empty() -> bool { return size() == 0; }
      auto empty() -> bool { return v_ == space_ ; }

      // --------------------------------------
      // Funciones de acceso.
      // Sobrecarga de operadores operator[].
      // Da acceso de escritura y lectura a ese hueco del array.
      auto operator[](std::size_t idx) -> T& { return v_[idx]; } // Devuelvo un acceso de lectur al original T&.
      // Este operador [idx] no tiene o hace bound checking
      // no tiene control de límite de acceso
      // si un vector tiene 2 valores ([0], [1]) y llamo al tercer valor [2] no habíra problema, y da acceso ahí
      // esto corrompe la memoria y le podría añadir un valor
      auto at(std::size_t idx) -> T& // Aquí tenemos ya comprobación de rango. Con Bound Cheking
      {
         // if (size() <= idx) {
         if ( idx >= size()) {
            throw std::out_of_range{};
         }
         return v_[idx]; 
      }

      // Begin() devuelve un iterador (pujntero de tipo T) a la primer casilla del vector. No una referencia. Donde estaría el primer elemento si hay alguno.
      auto begin() -> T* { return v_; }
      // End() centinela si llegas a esta dirección es porque te has salido fuera del rango de memoria.
      auto end() -> T* { return space_; }

      // Introducción al fondo de nuevos elementos de tipo T.
      auto push_back(T val) -> void
      {
         // Situaciones
         // 1. Cuando el vector esté vacio reserva dos casillas.
         // Tanto si el vector no tiene nada como si tiene cosas y he agotado la capacidad reservada
         if (space_ == last_) { // capacidad agotada o primera vez que se invoca push_back
            // si es vacio -> reserva dos
            // si se queda sin capacidad. Reserva capacidad actual * 2.
            auto const cp = capacity(); // capoacidad actual del array // capacidad si esta vacio es 0 y si v_ y las es igual tiene un valor.
            // auto new_cp = std::size_t{0};
            // if (cp == 0) {
            //    new_cp = 2;
            // } else {
            //    new_cp = 2*cp;
            // }
            auto const new_cp = (cp == 0) ? std::size_t{2} : 2*cp; // capacidad del nuevo array.
            // nuevo bloque de memoriua con tantos objetos tipo T como indique new_cp;
            T* new_block = new T[new_cp]; // Devuelvo un puntero a donde nace tu nuevo array.
            // SI no encuentra que devolver va a devolver bad_alloc.

            // GARANTIA FUERTE ANTE EXCEPCIONES. Si la función no puede cumplir lo que pretende, se lanza la excepcioón y el vector original se queda igual que como estaba.
            // No se van a corromper tus datos.
            try {
               // voy al vector orioginal
               for (auto i = std::size_t{0}; i < cp; ++i) {
                  new_block[i] = v_[i];
               }
               // meto el valor nuevo
               new_block[cp] = val; //me voy a la casilla siguiente que seguiria a mi array original que antes no me cabía.
            }
            catch (...) { // de lanzarse una excepción. Destruimos el array y relanzamos la excepcion.
               delete[] new_block;
               throw; // coge la excepcion actual capturada por catch y la relanza.
            }
            
            // Destruimos el array original y reasignamos los punteros:
            delete[] v_;
            v_ = new_block;
            space_ = new_block + cp + 1;
            last_ = new_block + new_cp;

         }
         else {
            // Cuando no se agota la capacidad. Es posible introducir elementos.
            // Disponemos de al menos una casilla libre para esta oiperaicón push_back
            *space_ = val; // * operador de indiferencia o referenmcia a la casilla. Entro a la casilla y asigno el valor.
            ++space_;
         }
      }

};

auto main() -> int
{
   // auto words = std::vector<std::string>{}; // Vector de palabras inicialmente vacio.
   auto words = Vector<std::string>{}; // Vector de palabras inicialmente vacio.
   // auto words = Vector<double>{};

   // try {
   //    words.push_back("Hello");
   //    words.push_back("World");
   //    words.at(2) = "!";
   // } catch () {

   // }

   auto wrd = std::string{};

   // Mientra que no se haga CTRL+Z o CTRL+D, introduce palabras.
   auto mssg = [] () {
      std::print("Introduce una palabra (CTRL+Z Windows o CTRL+D UNIX): ");
      return true;
   };

   while (mssg() and std::getline(std::cin, wrd))
   {
      words.push_back(wrd);
      // std::print("Introduce una palabra: ");
   }
   
   std::ranges::sort(words);

   for (auto it = words.begin(); it != words.end(); ++it)
   // for (std::string  const& w : words)
   {
      // std::println("{}", w);
      std::println("{}", *it);
   }

   return EXIT_SUCCESS;
}