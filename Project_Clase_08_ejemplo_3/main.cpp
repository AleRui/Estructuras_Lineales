#include <algorithm> // ordenar, buscar, hacer particiones... algoritmos de ordenacion
#include <cstdlib>   // Biblioteca Standard de C, contiene la función std::tolower (paso a minúscula)
#include <fstream>   // flujos I/O a ficheros
#include <map>       // contenedor asociativo de clave única ordenada
#include <print>     // imprimir en consola/terminal
#include <ranges>    // vistas como chunk_by
#include <string>    // trabajar con string
#include <vector>    // contenedor secuencial de elementos contiguos. Amigo memoria cache.

#include <boost/tokenizer.hpp> // Biblioteca externa boost

///////////////////////////////////////////////////////////////////////

auto file_to_string(std::string filename) -> std::string
{
   using namespace std;
   
   auto in = ifstream{};
   in.exceptions(ifstream::failbit | ifstream::badbit);
   in.open(filename, ios::binary);

   auto contents = string{};
   in.seekg(0, ios::end);
   if (auto const sz = streamoff{in.tellg()}; sz > 0) { // sz es entero con signo
      contents.resize(sz); // bloque de memoria contigua desde C++11
      in.seekg(0, ios::beg);
      in.read(&contents[0], sz);
   }
   return contents;
}

///////////////////////////////////////////////////////////////////////

auto main() -> int
{
   auto text = file_to_string("../../ulysses_joyce_4300-0.txt"); // String cono todo el texto, situado en el free store.

   auto tokens = std::vector<std::string>{};
   for (std::string tkn : boost::tokenizer{text}) {
      tokens.push_back(tkn);
   }

   std::println("{}", tokens.size());
   std::println("{}", tokens.at(1));

   return EXIT_SUCCESS;
}