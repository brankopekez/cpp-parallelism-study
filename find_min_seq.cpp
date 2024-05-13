#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

auto find_min(const std::vector<int> &v) {
  int min = v[0];
  for (int i = 1; i < v.size(); ++i) {
    if (v[i] < min) {
      min = v[i];
    }
  }
  return min;
}

int main() {
  constexpr int n = 10000;
  std::vector<int> v(n);
  std::iota(v.begin(), v.end(), 1);

  auto start = std::chrono::steady_clock::now();
  auto min = find_min(v);
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::micro>(end - start).count();
  std::cout << "Najmanji element: " << min << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " us" << std::endl;
  return 0;
}