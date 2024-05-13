#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

auto is_prime(unsigned n) {
  if (n < 2)
    return false;
  if (n == 2)
    return true;
  if (n % 2 == 0)
    return false;
  for (auto i = 3u; (i * i) <= n; i += 2) {
    if ((n % i) == 0) {
      return false;
    }
  }
  return true;
};

void partial(const std::vector<int> &v, size_t start, size_t end, int &p_count) {
  int count = 0;
  for (size_t i = start; i < end; ++i) {
    if (is_prime(v[i]))
      ++count;
  }
  p_count = count;
}

int count_primes_parallel(const std::vector<int> &v) {
  auto n_cores = std::thread::hardware_concurrency();
  auto n_threads = std::max(n_cores, 1u);
  auto block_size = v.size() / n_threads;
  std::vector<std::thread> threads(n_threads);
  std::vector<int> partial_counts(n_threads);

  for (size_t i = 0; i < n_threads; ++i) {
    auto start = i * block_size;
    auto end = std::min((i + 1) * block_size, v.size());
    threads[i] =
        std::thread(partial, std::ref(v), start, end, std::ref(partial_counts[i]));
  }
  for (auto &thread : threads) {
    thread.join();
  }
  int total_count = 0;
  for (int count : partial_counts) {
    total_count += count;
  }
  return total_count;
}

int main() {
  constexpr int n = 10000;
  std::vector<int> v(n);
  std::iota(v.begin(), v.end(), 1);

  auto start = std::chrono::steady_clock::now();
  auto sum = count_primes_parallel(v);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::micro>(end - start).count();
  std::cout << "Prostih elemenata: " << sum << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " us" << std::endl;
  return 0;
}
