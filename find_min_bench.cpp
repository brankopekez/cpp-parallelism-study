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

auto find_min(const std::vector<int> &v) {
  int min = v[0];
  for (int i = 1; i < v.size(); ++i) {
    if (v[i] < min) {
      min = v[i];
    }
  }
  return min;
}

void find_min_partial(const std::vector<int> &v, size_t start, size_t end,
                      int &result) {
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
    threads[i] = std::thread(find_min_partial, std::ref(v), start, end,
                             std::ref(min_results[i]));
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

void bm_sequential(benchmark::State &state) {
  auto src = gen_vec(state.range(0));
  for (auto _ : state) {
    benchmark::DoNotOptimize(find_min(src));
  }
}
void bm_parallel(benchmark::State &state) {
  auto src = gen_vec(state.range(0));
  for (auto _ : state) {
    benchmark::DoNotOptimize(find_min_parallel(src));
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