#include <chrono>
#include <iomanip>
#include <iostream>

auto calculate_pi(size_t n) {
  long double sum = 0.0L;
  long double delta_x = 1.0L / n;

  for (auto i = 0; i < n; ++i) {
    long double x = (i + 0.5L) * delta_x;
    sum += 4.0L / (1.0L + x * x);
  }

  return sum * delta_x;
}

int main() {
  constexpr size_t n = 1'000'000'000;
  auto start = std::chrono::steady_clock::now();
  auto pi = calculate_pi(n);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::milli>(end - start).count();

  const auto default_precision{std::cout.precision()};
  std::cout << "Izračunata vrijednost pi: " << std::setprecision(15) << pi
            << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed
            << std::setprecision(default_precision) << d << " ms" << std::endl;
  return 0;
}