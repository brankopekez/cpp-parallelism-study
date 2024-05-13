#include <algorithm>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

void partial(size_t start, size_t end, size_t n, long double &partial_sum) {
  long double sum = 0.0L;
  long double delta_x = 1.0L / n;
  for (int i = start; i < end; ++i) {
    long double x = (i + 0.5L) * delta_x;
    sum += 4.0L / (1.0L + x * x);
  }
  partial_sum = sum;
}

auto calculate_pi_parallel(size_t n) {
  auto n_cores = std::thread::hardware_concurrency();
  auto n_threads = std::max(n_cores, 1u);
  auto block_size = n / n_threads;
  std::vector<std::thread> threads(n_threads);
  std::vector<long double> partial_sums(n_threads);

  for (auto i = 0; i < n_threads; ++i) {
    auto start = i * block_size;
    auto end = std::min((i + 1) * block_size, n);
    threads[i] = std::thread(partial, start, end, n, std::ref(partial_sums[i]));
  }
  for (auto &thread : threads) {
    thread.join();
  }
  long double total_sum = 0.0L;
  for (auto partial_sum : partial_sums) {
    total_sum += partial_sum;
  }
  return total_sum * (1.0L / n);
}

int main() {
  constexpr size_t n = 1'000'000'000;
  auto start = std::chrono::steady_clock::now();
  auto pi = calculate_pi_parallel(n);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::milli>(end - start).count();
  const auto default_precision{std::cout.precision()};
  std::cout << "Izračunata vrijednost pi: " << std::setprecision(15) << pi
            << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed
            << std::setprecision(default_precision) << d << " ms" << std::endl;
  return 0;
}