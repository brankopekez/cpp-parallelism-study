#include <algorithm>
#include <benchmark/benchmark.h>
#include <chrono>
#include <execution>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

auto gen_vec(size_t n) {
  std::vector<int> v(n);
  std::iota(v.begin(), v.end(), 1);
  return v;
}

auto is_prime = [](unsigned n) {
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

auto count_primes(const std::vector<int> &v) {
  return std::count_if(v.begin(), v.end(), is_prime);
}

auto count_primes_parallel(const std::vector<int> &v) {
  return std::count_if(std::execution::par, v.begin(), v.end(), is_prime);
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
    benchmark::DoNotOptimize(std::count_if(std::execution::par, v.begin(), v.end(), is_prime));
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