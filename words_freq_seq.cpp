#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

std::map<std::string, int> broj_pojavljivanja_rijeci;

void broj_rijeci(const std::filesystem::path &putanja) {
  // Čitanje cijelog sadržaja datoteke
  std::ifstream datoteka(putanja);
  std::stringstream bafer;
  bafer << datoteka.rdbuf();
  std::string sadrzaj_datoteke = bafer.str();

  // Uklanjanje interpunkcijskih znakova i pretvaranje u mala slova
  std::transform(sadrzaj_datoteke.begin(), sadrzaj_datoteke.end(),
                 sadrzaj_datoteke.begin(), ::tolower);
  sadrzaj_datoteke.erase(std::remove_if(sadrzaj_datoteke.begin(),
                                        sadrzaj_datoteke.end(), ::ispunct),
                         sadrzaj_datoteke.end());

  std::string rijec;
  std::istringstream iss(sadrzaj_datoteke);
  while (iss >> rijec)
    broj_pojavljivanja_rijeci[rijec]++;
}

int main() {
  auto pocetak = std::chrono::steady_clock::now();
  std::string direktorijum = "books";
  for (auto const &stavka :
       std::filesystem::directory_iterator{direktorijum})
    if (stavka.is_regular_file())
      broj_rijeci(stavka.path());

  // Mjerenje vremena izvršavanja
  auto kraj = std::chrono::steady_clock::now();
  auto trajanje = std::chrono::duration_cast<std::chrono::milliseconds>(
      kraj - pocetak);

  // Sortiranje mape prema broju pojavljivanja riječi
  std::map<int, std::string, std::greater<int>> sortirane_rijeci;
  for (const auto &par : broj_pojavljivanja_rijeci)
    sortirane_rijeci.insert(std::make_pair(par.second, par.first));

  // Ispis rezultata
  int brojac = 0;
  std::cout << "Najčešće riječi:\n";
  for (const auto &par : sortirane_rijeci)
    if (par.second.length() >= 5) {
      std::cout << par.second << ": " << par.first << " pojavljivanja\n";
      if (++brojac == 10)
        break;
    }

  // Ispis vremena izvršavanja
  std::cout << "Vrijeme izvršavanja: " << trajanje.count()
            << " milisekundi\n";

  return 0;
}
