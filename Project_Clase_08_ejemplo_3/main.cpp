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
   namespace stdr = std::ranges;
   namespace stdv = std::views;

   // 1. Carga del texto en momoria dentro de un string.
   auto text = file_to_string("../../ulysses_joyce_4300-0.txt"); // String cono todo el texto, situado en el free store.

   // 2. Pasar el texto a minúscula.
   // Landa function pasara caractewrtes a minuscula.
   // auto to_lower = [] (unsigned char c) { return std::tolower(c); };
   // Sobreescribe la variable text, hay que decirle desde donde con text.begin().
   // stdr::transform(text, text.begin(), to_lower);
   // Combinación de lo anterior.
   // (puede ser poco profesional el tratamiento realizado la texto)
   stdr::transform(text, text.begin(), [] (unsigned char c) { return std::tolower(c); });


   // 3. Construimos vector de string.
   // Mismo orden que aparecen en el texto.
   // auto tokens = std::vector<std::string>{};
   // for (std::string tkn : boost::tokenizer{text}) {
   //    tokens.push_back(tkn);
   // }

   // function to dentro de library ranges introduce todos los strings en un vector de string.
   // Mismo funcionamiento que foreach anterior.
   auto tokens = stdr::to<std::vector<std::string>>(boost::tokenizer{text});

   std::println("{}", tokens.size());
   std::println("{}", tokens.at(1));

   // 4. Ordenar alfabéticamente el vector de tokens.
   stdr::sort(tokens);

   // 5. Preparamos un mapa para ser poblado.
   auto freq_same_token = std::map<int, std::vector<std::string>>{};

   // 6. Troceamos el vector, en subconjuntos por palabras idénticas.
   // Inicializamos la Lamda dentro del bucle foreach.
   for (
      auto same_token = [] (std::string const& t1, std::string const& t2) { return t1 == t2; };
      auto token_chunk : tokens | stdv::chunk_by( same_token )
   ){
      // Contar repeticiones en cada trozo.
      // auto const freq = token_chunk.size();
      auto const freq = stdr::distance(token_chunk);
      auto const tkn = *stdr::begin(token_chunk); // para coger datos del iterador, con una copia, desreferenciar.
      // auto const tkn = stdr::begin(token_chunk)->parametro; // para coger datos del iterador, con su interfaz publica, desreferenciar.

      // Registrames pareja clave valor.
      freq_same_token[freq].push_back(tkn);
   }

   return EXIT_SUCCESS;
}