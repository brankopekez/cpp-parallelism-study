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

void copy_primes(const std::vector<int> &src, std::vector<int> &dst) {
  for (auto num : src) {
    if (is_prime(num))
      dst.push_back(num);
  }
}

int main() {
  constexpr int n = 10'000'000;
  std::vector<int> src(n);
  std::iota(src.begin(), src.end(), 1);

  std::vector<int> dst;

  auto start = std::chrono::steady_clock::now();
  copy_primes(src, dst);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::milli>(end - start).count();
  std::cout << "Kopirano elemenata: " << dst.size() << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " ms" << std::endl;
  return 0;
}