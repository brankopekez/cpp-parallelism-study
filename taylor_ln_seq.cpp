#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>

long double taylor_ln(long double x, size_t n) {
  long double result = 0.0;
  for (auto i = 1; i <= n; ++i) {
    result += pow(-1, i - 1) * pow(x - 1, i) / i;
  }
  return result;
}

int main() {
  long double x = 2.0;                // Vrijednost za koju se računa logaritam
  constexpr size_t n = 1'000'000'000; // Red Tejlorovog polinoma
  auto start = std::chrono::steady_clock::now();
  auto ln_x = taylor_ln(x, n);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<long double>(end - start).count();
  const auto default_precision{std::cout.precision()};
  std::cout << "Prirodni logaritam od " << x << " je približno "
            << std::setprecision(15) << ln_x << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed
            << std::setprecision(default_precision) << d << " s" << std::endl;
  return 0;
}
