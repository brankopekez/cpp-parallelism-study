#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

long long int find_sum(const std::vector<int> &v) {
  long long int sum = 0;
  for (size_t i = 0; i < v.size(); ++i) {
    sum += v[i];
  }
  return sum;
}

int main() {
  constexpr int n = 100'000;
  std::vector<int> v(n);
  std::iota(v.begin(), v.end(), 1);

  auto start = std::chrono::steady_clock::now();
  auto sum = find_sum(v);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::micro>(end - start).count();
  std::cout << "Suma elementa: " << sum << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " us" << std::endl;
  return 0;
}