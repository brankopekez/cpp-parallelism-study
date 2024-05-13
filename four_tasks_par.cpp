#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

void f1(const std::vector<int> &v, double &r) {
  r = 0;
  for (int num : v)
    if (num % 2 == 0 && num % 7 == 0)
      r += std::cbrt(num);
}
void f2(const std::vector<int> &v, double &r) {
  r = 0;
  for (int num : v)
    if (num % 3 == 0 && num % 8 == 0)
      r += std::cbrt(num);
}
void f3(const std::vector<int> &v, double &r) {
  r = 0;
  for (int num : v)
    if (num % 3 == 0 && num % 7 == 0)
      r += std::cbrt(num);
}
void f4(const std::vector<int> &v, double &r) {
  r = 0;
  for (int num : v)
    if (num % 4 == 0 && num % 9 == 0)
      r += std::cbrt(num);
}
int main() {
  constexpr int n = 100'000'000;
  std::vector<int> v(n);
  std::iota(v.begin(), v.end(), 1);
  double r1 = 0, r2 = 0, r3 = 0, r4 = 0;
  auto start = std::chrono::steady_clock::now();
  std::thread thread1(f1, std::ref(v), std::ref(r1));
  std::thread thread2(f2, std::ref(v), std::ref(r2));
  std::thread thread3(f3, std::ref(v), std::ref(r3));
  std::thread thread4(f4, std::ref(v), std::ref(r4));
  thread1.join();  thread2.join();  thread3.join();  thread4.join();
  auto end = std::chrono::steady_clock::now();
  auto d = std::chrono::duration<double, std::milli>(end - start).count();
  std::cout << "Suma trećih korijena brojeva djeljivih s 2 i 7: " << r1 << std::endl;
  std::cout << "Suma trećih korijena brojeva djeljivih s 3 i 8: " << r2 << std::endl;
  std::cout << "Suma trećih korijena brojeva djeljivih s 3 i 7: " << r3 << std::endl;
  std::cout << "Suma trećih korijena brojeva djeljivih s 4 i 9: " << r4 << std::endl;
  std::cout << "Proteklo vrijeme: " << std::fixed << d << " ms" << std::endl;
  return 0;
}