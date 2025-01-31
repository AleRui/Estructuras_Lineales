#include <algorithm>  // algoritmos de partición, ordenación, etc
#include <cstdlib>    // contiene EXIT_SUCCESS y EXIT_FAILURE
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
   std::string name;    // nombre
   std::string country; // país
   int year;            // año victoria
};

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
   // Cargamos la informaicón del Json.
   auto winners = get_from_jsonl<Player>("../../roland_garros.jsonl") | stdr::to<std::vector>();

   auto const min_possible_year = winners.begin()->year;
   auto const max_possible_year = (winners.end() - 1)->year;

   // 1. Encontrar jugadores entre un rango de fechas:
   std::print("Introduce primer año del a rango a elegir (entre los años {} y {}): ", min_possible_year, max_possible_year);
   auto year_1_max = int{0};
   (std::cin >> year_1_max).ignore();

   std::print("Introduce segundo año del a rango a elegir (entre los años {} y {}): ", min_possible_year, max_possible_year);
   auto year_2_min = int{0};
   (std::cin >> year_2_min).ignore();

   if (year_1_max < year_2_min)
   {
      int new_max = year_2_min;
      year_2_min = year_1_max;
      year_1_max = new_max;
   }

   std::println("Menor año elegido: {}", year_1_max);
   std::println("Mayor año elegido: {}", year_2_min);

   auto const search_iter_min_year = stdr::find(winners, year_2_min, &Player::year);
   auto const search_iter_max_year = stdr::find(winners, year_1_max, &Player::year);

   if (
       search_iter_min_year != winners.end()
       && search_iter_max_year != winners.end()
   )
   {
      auto const &min_year = search_iter_min_year->year;
      auto const &max_year = search_iter_max_year->year;

      stdr::sort(winners, {}, &Player::year);

      auto between_min_and_max_year = [&](Player const &player) -> bool { return (player.year >= min_year && player.year <= max_year); };

      auto [first_iter_after_min_year, second_iter_same_end] = stdr::stable_partition(search_iter_min_year + 1, search_iter_max_year, between_min_and_max_year);

      std::println("------------------------------------");
      std::println("Ganadores entre {} y {}", year_2_min, year_1_max);
      for (Player const& winner : std::ranges::subrange{search_iter_min_year, first_iter_after_min_year + 1})
      {
         std::println("{:>20} | {} - {}", winner.name, winner.country, winner.year);
      }
   }
   else
   {
      std::println("------------------------------------");
      std::println("El periodo de años elegido entre {} y {} no correspondo al periodo posible que es entre {} y {}.", year_2_min, year_1_max, min_possible_year, max_possible_year);
   }

   // -------------------------------------------------
   // 2. Solicitamos un país de álbum al usuario:
   std::print("Introduce las siglas de un país): ");
   auto country_code = std::string{};
   std::getline(std::cin, country_code);

   auto const search_iter_country = stdr::find(winners, country_code, &Player::country);
   if (search_iter_country != winners.end())
   {
      stdr::sort(winners, std::less{}, &Player::country);
      auto [part_iter_1, part_iter_2] = stdr::equal_range(winners, country_code, stdr::less{}, &Player::country);

      auto country_winners = stdr::subrange{part_iter_1, part_iter_2};
      stdr::sort(country_winners, std::ranges::less{}, &Player::year);

      std::println("------------------------------------");
      std::println("Ganadores del país con las siglas: {}", country_code);
      for (Player const &winner : country_winners)
      {
         std::println("{:>20} | {} - {}", winner.name, winner.country, winner.year);
      }
   } else {
      std::println("------------------------------------");
      std::println("Las siglas de páis elegido no es encuentra estre los código disponibles.", country_code);
   }

   return EXIT_SUCCESS;
}