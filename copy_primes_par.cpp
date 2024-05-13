#include <chrono>
#include <iostream>
#include <mutex>
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

void copy_primes_partial(const std::vector<int> &src, std::vector<int> &dst,
                         size_t start, size_t end, std::mutex &mtx) {
  for (size_t i = start; i < end; ++i) {
    if (is_prime(src[i])) {
      std::lock_guard<std::mutex> lock(mtx);
      dst.push_back(src[i]);
    }
  }
}

void copy_primes_parallel(const std::vector<int> &src, std::vector<int> &dst) {
  auto n_cores = std::thread::hardware_concurrency();
  auto n_threads = std::max(n_cores, 1u);
  auto block_size = src.size() / n_threads;
  std::vector<std::thread> threads(n_threads);
  std::mutex mtx;

  for (size_t i = 0; i < n_threads; ++i) {
    auto start = i * block_size;
    auto end = std::min((i + 1) * block_size, src.size());
    threads[i] = std::thread(copy_primes_partial, std::ref(src), std::ref(dst), start,
                             end, std::ref(mtx));
  }
  for (auto &thread : threads) {
    thread.join();
  }
}

int main() {
  constexpr int n = 10'000'000;
  std::vector<int> src(n);
  std::iota(src.begin(), src.end(), 1);
  std::vector<int> dst;
  std::mutex mtx;
  auto start = std::chrono::steady_clock::now();
  copy_primes_parallel(src, dst);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::milli>(end - start).count();
  std::cout << "Kopirano elemenata: " << dst.size() << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " ms" << std::endl;
  return 0;
}