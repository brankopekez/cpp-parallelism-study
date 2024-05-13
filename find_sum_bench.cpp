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

long long int find_sum(const std::vector<int> &v) {
  long long int sum = 0;
  for (size_t i = 0; i < v.size(); ++i) {
    sum += v[i];
  }
  return sum;
}

void calculate_partial_sum(const std::vector<int> &v, size_t start, size_t end,
                           long long int &partial_sum) {
  long long int sum = 0;
  for (auto i = start; i < end; ++i) {
    sum += v[i];
  }
  partial_sum = sum;
}

auto find_sum_parallel(const std::vector<int> &v) {
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

void bm_sequential(benchmark::State &state) {
  auto src = gen_vec(state.range(0));
  for (auto _ : state) {
    benchmark::DoNotOptimize(find_sum(src));
  }
}
void bm_parallel(benchmark::State &state) {
  auto src = gen_vec(state.range(0));
  for (auto _ : state) {
    benchmark::DoNotOptimize(find_sum_parallel(src));
  }
}

void CustomArguments(benchmark::internal::Benchmark *b) {
  b->MeasureProcessCPUTime()
      ->UseRealTime()
      ->Unit(benchmark::kMillisecond)
      ->RangeMultiplier(10)
      ->Range(1'000, 1'000'000'000);
}

BENCHMARK(bm_sequential)->Apply(CustomArguments);

BENCHMARK(bm_parallel)->Apply(CustomArguments);

BENCHMARK_MAIN();