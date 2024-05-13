#include <chrono>
#include <iostream>
#include <numeric>
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

int count_primes(const std::vector<int> &v) {
  int count = 0;
  for (int num : v) {
    if (is_prime(num))
      ++count;
  }
  return count;
}

int main() {
  constexpr int n = 10000;
  std::vector<int> v(n);
  std::iota(v.begin(), v.end(), 1);

  auto start = std::chrono::steady_clock::now();
  auto sum = count_primes(v);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::micro>(end - start).count();
  std::cout << "Prostih elemenata: " << sum << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " us" << std::endl;
  return 0;
}