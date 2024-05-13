#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

void calculate_partial_sum(const std::vector<int> &v, size_t start, size_t end,
                           long long int &partial_sum) {
  long long int sum = 0;
  for (auto i = start; i < end; ++i) {
    sum += v[i];
  }
  partial_sum = sum;
}

auto find_sum(const std::vector<int> &v) {
  auto n_cores = size_t{std::thread::hardware_concurrency()};
  auto n_threads = std::max(n_cores, size_t{1});
  auto block_size = v.size() / n_threads;
  std::vector<std::thread> threads(n_threads);
  std::vector<long long int> partial_sums(n_threads);
  for (auto i = 0; i < n_threads; ++i) {
    auto start = i * block_size;
    auto end = std::min((i + 1) * block_size, v.size());
    threads[i] = std::thread(calculate_partial_sum, std::ref(v), start, end,
                             std::ref(partial_sums[i]));
  }
  for (auto &thread : threads) {
    thread.join();
  }
  long long int final_sum = 0;
  for (auto i = 0; i < n_threads; ++i) {
    final_sum += partial_sums[i];
  }
  return final_sum;
}

int main() {
  constexpr int n = 100'000;
  std::vector<int> v(n);
  std::iota(v.begin(), v.end(), 1);

  auto start = std::chrono::steady_clock::now();
  auto sum = find_sum(std::ref(v));
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::micro>(end - start).count();
  std::cout << "Suma elementa: " << sum << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " us" << std::endl;
  return 0;
}