#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

std::vector<std::filesystem::path> lista_datoteka;
int brojac = 0;
std::mutex brava;
std::map<std::string, int> zajednicka_mapa;

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
  std::map<std::string, int> lokalna_mapa;
  while (iss >> rijec) {
    lokalna_mapa[rijec]++;
  }

  // Prenos lokalne mape u zajedničku mapu
  {
    std::lock_guard<std::mutex> lk(brava);
    for (const auto &par : lokalna_mapa) {
      zajednicka_mapa[par.first] += par.second;
    }
  }
}

void funkcija_niti() {
  int indeks_u_listi = 0;
  {
    std::lock_guard<std::mutex> lk(brava);
    if (brojac >= lista_datoteka.size())
      return;
    indeks_u_listi = brojac;
    ++brojac;
  }
  while (true) {
    broj_rijeci(lista_datoteka[indeks_u_listi]);
    {
      std::lock_guard<std::mutex> lk(brava);
      if (brojac >= lista_datoteka.size())
        break;
      indeks_u_listi = brojac;
      ++brojac;
    }
  }
}

int main(int argc, char *argv[]) {
  constexpr int max_broj_niti = 4;
  auto pocetak = std::chrono::steady_clock::now();
  std::string direktorijum = "books";
  for (auto const &stavka :
       std::filesystem::directory_iterator{direktorijum}) {
    if (stavka.is_regular_file()) {
      lista_datoteka.emplace_back(stavka.path());
    }
  }

  std::thread niti[max_broj_niti];
  for (int i = 0; i < max_broj_niti; ++i) {
    niti[i] = std::thread(&funkcija_niti);
  }
  funkcija_niti();

  for (int i = 0; i < max_broj_niti; ++i) {
    niti[i].join();
  }

  // Mjerenje vremena izvršavanja
  auto kraj = std::chrono::steady_clock::now();
  auto trajanje = std::chrono::duration_cast<std::chrono::milliseconds>(
      kraj - pocetak);

  // Sortiranje mape prema broju pojavljivanja riječi
  std::map<int, std::string, std::greater<int>> sortirane_rijeci;
  for (const auto &par : zajednicka_mapa) {
    sortirane_rijeci.insert(std::make_pair(par.second, par.first));
  }

  // Ispis rezultata
  std::cout << "Najčešće riječi:\n";
  int brojac_najcescih_rijeci = 0;
  for (const auto &par : sortirane_rijeci) {
    if (par.second.length() >= 5) {
      std::cout << par.second << ": " << par.first << " pojavljivanja\n";
      if (++brojac_najcescih_rijeci == 10)
        break;
    }
  }

  // Ispis vremena izvršavanja
  std::cout << "Vrijeme izvršavanja: " << trajanje.count()
            << " milisekundi\n";

  return 0;
}