#include <algorithm>
#include <benchmark/benchmark.h>
#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

auto gen_vec(size_t n) {
  std::vector<int> v(n);
  std::iota(v.begin(), v.end(), 1);
  return v;
}

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

int count_primes(const std::vector<int> &v) {
  int count = 0;
  for (int num : v) {
    if (is_prime(num))
      ++count;
  }
  return count;
}

void count_primes_partial(const std::vector<int> &v, size_t start, size_t end,
                          int &partial_count) {
  int count = 0;
  for (size_t i = start; i < end; ++i) {
    if (is_prime(v[i]))
      ++count;
  }
  partial_count = count;
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
    threads[i] = std::thread(count_primes_partial, std::ref(v), start, end,
                             std::ref(partial_counts[i]));
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

void bm_sequential(benchmark::State &state) {
  auto src = gen_vec(state.range(0));
  for (auto _ : state) {
    benchmark::DoNotOptimize(count_primes(src));
  }
}
void bm_parallel(benchmark::State &state) {
  auto src = gen_vec(state.range(0));
  for (auto _ : state) {
    benchmark::DoNotOptimize(count_primes_parallel(src));
  }
}

void CustomArguments(benchmark::internal::Benchmark *b) {
  b->MeasureProcessCPUTime()
      ->UseRealTime()
      ->Unit(benchmark::kMillisecond)
      ->RangeMultiplier(10)
      ->Range(1'000, 100'000'000);
}

BENCHMARK(bm_sequential)->Apply(CustomArguments);

BENCHMARK(bm_parallel)->Apply(CustomArguments);

BENCHMARK_MAIN();