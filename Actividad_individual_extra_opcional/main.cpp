#include <cstdlib>    // contiene EXIT_SUCCESS y EXIT_FAILURE
#include <algorithm>  // algoritmos de partición, ordenación, etc
#include <fstream>    // flujos i/o a ficheros
#include <functional> // contiene la función std::not_fn para negar lambdas
#include <generator>  // generador asociado a corrutina
#include <iostream>   // contiene std::cin (flujo de entrada estándar)
#include <print>
#include <ranges>
#include <string>
#include <vector> // Contenedor, ayuda a la cache.

// biblioteca no-estádar:
#include <nlohmann/json.hpp>

namespace stdr = std::ranges;
namespace stdv = std::views;

struct Player
{
   std::string name;    // título
   std::string country; // país
   int year;            // año victoria
};

// MACRO asociada
// macro asociada de la biblioteca NLohmann's JSON:
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Player, name, country, year)

// Functions --------------------------------------

template <typename T>
auto get_from_jsonl(std::filesystem::path pth) -> std::generator<T>
{
   if (pth.extension() != ".jsonl")
   {
      throw std::ios::failure{"the extension of the JSON Lines file must be '.jsonl' "};
   }

   auto ifs = std::ifstream{pth, std::ios::binary};

   if (!ifs)
   {
      throw std::ios::failure{"unable to open the JSON Lines file"};
   }

   auto ln = std::string{};
   while (std::getline(ifs, ln))
   {
      co_yield nlohmann::json::parse(ln).get<T>();
   }
}

// -------------------------------------------------

auto main() -> int
{
   // solicitamos un título de álbum al usuario:
   std::print("Introduce primer año: ");
   auto year_1_max = int{0};
   (std::cin >> year_1_max).ignore();

   std::print("Introduce segundo año: ");
   auto year_2_min = int{0};
   (std::cin >> year_2_min).ignore();

   if (year_1_max < year_2_min)
   {
      int new_max = year_2_min;
      year_2_min = year_1_max;
      year_1_max = new_max;
   }

   std::println("year_1_max: {}", year_1_max);
   std::println("year_2_min: {}", year_2_min);

   auto winners = get_from_jsonl<Player>("../../roland_garros.jsonl") | stdr::to<std::vector>();

   stdr::sort(winners, {}, &Player::year);

   auto elements_less_min = [&](Player const &player) -> bool { return player.year < year_2_min; };
   const auto partition_previous = stdr::stable_partition(winners, elements_less_min);

   std::println("------------- Previous -------------");
   auto counter_previous = 0;
   for (Player winner_previous : std::ranges::subrange{winners.begin(), partition_previous.begin()})
   {
      std::println("Player: {}, Country: {}, Year: {}", winner_previous.name, winner_previous.country, winner_previous.year);
      counter_previous++;
   }
   std::println("counter_previous: {}", counter_previous);



   // auto elements_more_max = [&](Player const &player) -> bool { return player.year > year_1_max; };
   // const auto partition_leater = stdr::stable_partition(winners, elements_more_max);

   // auto counter_leater = 0;
   // std::println("------------- Later    -------------");
   // for (Player winner_leater : std::ranges::subrange{partition_leater.begin(), winners.end()})
   // {
   //    std::println("Player: {}, Country: {}, Year: {}", winner_leater.name, winner_leater.country, winner_leater.year);
   //    counter_leater++;
   // }
   // std::println("counter_leater: {}", counter_leater);




   std::println("------------------------------------");
   std::println("Total winners: {}", winners.size());

   return EXIT_SUCCESS;
}