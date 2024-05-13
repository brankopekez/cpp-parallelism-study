#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

void taylor_ln_partial(long double x, size_t start, size_t end, size_t n,
                       long double &partial_sum) {
  partial_sum = 0.0;
  for (auto i = start; i < end; ++i) {
    partial_sum += pow(-1, i - 1) * pow(x - 1, i) / i;
  }
}

auto taylor_ln_parallel(long double x, size_t n, size_t n_threads) {
  auto block_size = n / n_threads;
  std::vector<std::thread> threads;
  std::vector<long double> partial_sums(n_threads);

  for (size_t i = 0; i < n_threads; ++i) {
    size_t start = i * block_size;
    size_t end = std::min((i + 1) * block_size, n);
    threads.emplace_back(taylor_ln_partial, x, start + 1, end + 1, n,
                         std::ref(partial_sums[i]));
  }
  for (auto &thread : threads) {
    thread.join();
  }
  long double total_sum = 0.0;
  for (auto sum : partial_sums) {
    total_sum += sum;
  }
  return total_sum;
}

int main() {
  long double x = 2.0;
  constexpr size_t n = 1'000'000'000;
  constexpr size_t n_threads = 4;
  auto start = std::chrono::steady_clock::now();
  auto ln_x = taylor_ln_parallel(x, n, n_threads);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<long double>(end - start).count();
  const auto default_precision{std::cout.precision()};
  std::cout << "Prirodni logaritam od " << x << " je približno "
            << std::setprecision(15) << ln_x << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed
            << std::setprecision(default_precision) << d << " s" << std::endl;
  return 0;
}