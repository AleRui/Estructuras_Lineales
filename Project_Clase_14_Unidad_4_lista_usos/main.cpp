#include <cstdlib> // contiene EXIT_SUCCESS y EXIT_FAILURE
#include <iostream>
#include <list> // contiene std::list
#include <print>
#include <queue> // contiene std::queue
#include <stack> // contiene std::stack
#include <string>

// template <typename T>
// class Stack
// { // estructura LIFO
// private:
//    std : List<T> list_;
// public:
//    Stack() : list_{}
//    {
//       // constructor...
//    };

//    // acceso:
//    T const &top() const { return list_.front(); } // cima
//    T &top() { return list_.front(); }

//    // modificadores:
//    void push(T const &val) { list_.push_front(val); } // insertar sobre cima
//    void pop() { list_.pop_front(); }                  // eliminar cima

//    // capacidad:
//    auto empty() -> bool { return list_.empty(); }
//    std::size_t size() -> std::size_t { return list_.size(); }
// };

// template <typename T>
// class Queue
// { // estructura FIFO
//    List<T> list_;

// public:
//    Queue() = default;

//    // acceso:
//    T const &front() const { return list_.front(); } // primer elemento
//    T &front() { return list_.front(); }
//    T const &back() const { return list_.back(); } // último elemento
//    T &back() { return list_.back(); }

//    // modificadores:
//    void pop() { list_.pop_front(); }                 // eliminar el frente
//    void push(T const &val) { list_.push_back(val); } // insertar al fondo

//    // capacidad:
//    bool empty() const noexcept { return list_.empty(); }
//    std::size_t size() const noexcept { return list_.size(); }
// };

auto main() -> int
{
   // auto nums = std::list{1, 2, 3, 4, 5};

   // nums.push_back(6);
   // nums.push_front(0);
   // // Introducir algo e la lista entre cualquier nodo tiene el mismo coste.
   // // Aunque los benchmark del vector suelen supoerar a la lista
   // // a no ser que se trate de un volumen grande de datos.

   // for (auto const &num : nums)
   // {
   //    std::print("{}\n", num);
   // }

   auto nums = std::stack<int>{};

   nums.push(1);
   nums.push(2);
   nums.push(3);
   nums.top() = 7;

   std::println("Cima: {}", nums.top());

   // no existe begin() ni end() en la pila
   // tenemos que ir destruuyendo la pila pàra acceder a los valores anteriroes.

   // while (not nums.empty())
   while (!nums.empty())
   {
      std::println("{}", nums.top());
      nums.pop();
   }

   std::println("{}", nums.empty());
   // Si se llama a top con la pila estamos entrando en comportamiento indefinido.
   // Si se llama a top con la pila vacía, se puede producir un error de ejecución.
   // No podríamos usar vista ya que no podemos tener ranfo.

   // ----------------------------------------------
   // COLA Queue
   auto nums2 = std::queue<int>{};
   nums2.push(1);
   nums2.push(2);
   nums2.push(3);

   nums2.front() = 7;
   nums2.back() = 8;

   std::println("Frente: {}", nums2.front());
   std::println("Fondo: {}", nums2.back());

   while (!nums2.empty())
   {
      std::println("{}", nums2.front());
      nums2.pop();
   }

   std::println("{}", nums2.empty());

   // ----------------------------------------------
   // Averiguar palíndromos
   // auto word = std::string{"abcba"};

   // Solucion ineficiente. MALA en rendimiento
   // Replica en tres la info en memoria.
   std::println("Introduce una palabra: ");
   auto word = std::string{};
   std::getline(std::cin, word);

   auto pila = std::stack<char>{}; // pila de caracteres
   auto cola = std::queue<char>{}; // cola de caracteres

   for (char const &c : word)
   {
      pila.push(std::tolower(c));
      cola.push(std::tolower(c)); // Va a tener el mismo orden que el String.
   }

   auto es_palindromo = true;
   // while (es_palindromo and not pila.empty())
   while (es_palindromo && !pila.empty()) // pila.empty() = cola.empty() porque tienen mismo size.
   {
      if (pila.top() != cola.front())
      {
         es_palindromo = false;
      }
      pila.pop();
      cola.pop();
   }

   if (es_palindromo)
   {
      std::println("La palabra es un palíndromo.");
   }
   else
   {
      std::println("La palabra no es un palíndromo.");
   }


   return EXIT_SUCCESS;
}