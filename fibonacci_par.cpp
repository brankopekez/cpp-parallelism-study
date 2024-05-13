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

int main() {
  constexpr int n = 15;

  auto start = std::chrono::steady_clock::now();
  unsigned long long int fib = fibonacci_wrapper(n);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::micro>(end - start).count();

  std::cout << n << ". Fibonačijev broj: " << fib << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " us" << std::endl;
  return 0;
}
