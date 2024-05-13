#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

void partial(const std::vector<int> &v, size_t start, size_t end, int &result) {
  int min = v[start];
  for (int i = start + 1; i < end; ++i) {
    if (v[i] < min) {
      min = v[i];
    }
  }
  result = min;
}

int find_min_parallel(const std::vector<int> &v) {
  auto n_cores = size_t{std::thread::hardware_concurrency()};
  auto n_threads = std::max(n_cores, size_t{1});
  auto block_size = v.size() / n_threads;
  std::vector<std::thread> threads(n_threads);
  std::vector<int> min_results(n_threads);
  for (auto i = 0; i < n_threads; ++i) {
    auto start = i * block_size;
    auto end = std::min((i + 1) * block_size, v.size());
    threads[i] =
        std::thread(partial, std::ref(v), start, end, std::ref(min_results[i]));
  }
  for (auto &thread : threads) {
    thread.join();
  }
  auto global_min = min_results[0];
  for (auto i = 1; i < n_threads; ++i) {
    if (min_results[i] < global_min) {
      global_min = min_results[i];
    }
  }
  return global_min;
}

int main() {
  constexpr int n = 10000;
  std::vector<int> v(n);
  std::iota(v.begin(), v.end(), 1);
  auto start = std::chrono::steady_clock::now();
  auto min = find_min_parallel(std::ref(v));
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::micro>(end - start).count();
  std::cout << "Najmanji element: " << min << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " us" << std::endl;
  return 0;
}