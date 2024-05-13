#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

void f1(const std::vector<int> &v, double &r) {
  r = 0;
  for (auto num : v)
    if (num % 3 == 0 && num % 7 == 0) r += std::sqrt(num);
}
void f2(const std::vector<int> &v, double &r) {
  r = 0;
  for (auto num : v)
    if (num % 4 == 0 && num % 9 == 0) r += std::sqrt(num);
}

int main() {
  constexpr int n = 300'000'000;
  std::vector<int> v(n);
  std::iota(v.begin(), v.end(), 1);
  double r1 = 0, r2 = 0;
  auto start = std::chrono::steady_clock::now();
  std::thread t1([&]() { f1(v, r1); });
  std::thread t2([&]() { f2(v, r2); });
  t1.join();
  t2.join();
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::milli>(end - start).count();
  std::cout << "Suma korijena elemenata djeljivih sa 3 i 7: " << r1 << std::endl;
  std::cout << "Suma korijena elemenata djeljivih sa 4 i 9: " << r2 << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " ms" << std::endl;
  return 0;
}