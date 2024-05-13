#include <benchmark/benchmark.h> // Non-standard header
#include <chrono>
#include <iostream>
#include <thread>

unsigned long long int fibonacci(int n) {
  if (n <= 1) {
    return n;
  } else {
    return fibonacci(n - 1) + fibonacci(n - 2);
  }
}

unsigned long long int fibonacci_wrapper(int n) {
  if (n <= 1) {
    return n;
  } else {
    unsigned long long int fib1, fib2;
    std::thread t1([&]() { fib1 = fibonacci(n - 1); });
    std::thread t2([&]() { fib2 = fibonacci(n - 2); });

    t1.join();
    t2.join();

    return fib1 + fib2;
  }
}

static void bm_seq_fib(benchmark::State &state) {
  auto n = state.range(0);
  for (auto _ : state) {
    benchmark::DoNotOptimize(fibonacci(n));
  }
}

static void bm_par_fib(benchmark::State &state) {
  auto n = state.range(0);
  for (auto _ : state) {
    benchmark::DoNotOptimize(fibonacci_wrapper(n));
  }
}

void CustomArguments(benchmark::internal::Benchmark *b) {
  b->MeasureProcessCPUTime()           // Measure all threads
      ->UseRealTime()                  // Clock on the wall
      ->Unit(benchmark::kMillisecond); // Use ms
}

// BENCHMARK(bm_seq_fib)->Apply(CustomArguments)->Arg(15);
// BENCHMARK(bm_par_fib)->Apply(CustomArguments)->Arg(15);

// BENCHMARK(bm_seq_fib)->Apply(CustomArguments)->DenseRange(10, 50, 5);
// BENCHMARK(bm_par_fib)->Apply(CustomArguments)->DenseRange(10, 50, 5);

BENCHMARK(bm_seq_fib)->Apply(CustomArguments)->Arg(40);
BENCHMARK(bm_seq_fib)->Apply(CustomArguments)->Arg(39);
BENCHMARK(bm_seq_fib)->Apply(CustomArguments)->Arg(38);
BENCHMARK(bm_par_fib)->Apply(CustomArguments)->Arg(40);

BENCHMARK_MAIN();