#include <chrono>
#include <iostream>

unsigned long long int fibonacci(int n) {
  if (n <= 1) {
    return n;
  } else {
    return fibonacci(n - 1) + fibonacci(n - 2);
  }
}

int main() {
  constexpr int n = 15;

  auto start = std::chrono::steady_clock::now();
  unsigned long long int fib = fibonacci(n);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::micro>(end - start).count();

  std::cout << n << ". Fibonačijev broj: " << fib << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " us" << std::endl;
  return 0;
}
