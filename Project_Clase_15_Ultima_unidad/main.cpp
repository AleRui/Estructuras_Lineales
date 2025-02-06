#include <cstdlib> // contiene EXIT_SUCCESS y EXIT_FAILURE
#include <print>
#include <string>

// 4! = 4 * 3 * 2 * 1 = 24
// 4! = 4 * 3!
// 3! = 3 * 2!
// n! = n * (n-1)!

// 1! = 1 * (0!) = 1 * 1 // 0! = 1

// factoriales de un numero natural
auto factorial(int n) -> int // válida para n=>1.
{
   // Hay que establecer un caso base.
   // if (n == 0)
   if (n == 1) // porque sabemos que el factorial de 0 es 1.
   {
      return 1;
   }
   return n * factorial(n - 1);
}

auto main() -> int
{

   // RECURSIVIDAD
   // Factorial de un numero
   int n = 5;
   std::println("Factorial de {} es: {}", n, factorial(n));

   return EXIT_SUCCESS;
}