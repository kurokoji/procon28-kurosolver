#ifndef KUROUTIL_HPP
#define KUROUTIL_HPP

#include <iostream>
#include <vector>

namespace kuroutil {
// 立っているビット数を数える
int popcount(unsigned int v) {
  unsigned int count = (v & 0x55555555) + ((v >> 1) & 0x55555555);
  count = (count & 0x33333333) + ((count >> 2) & 0x33333333);
  count = (count & 0x0f0f0f0f) + ((count >> 4) & 0x0f0f0f0f);
  count = (count & 0x00ff00ff) + ((count >> 8) & 0x00ff00ff);
  return (count & 0x0000ffff) + ((count >> 16) & 0x0000ffff);
}

int popcount(unsigned long long int v) {
  unsigned long long int count = (v & 0x5555555555555555) + ((v >> 1) & 0x5555555555555555);
  count = (count & 0x3333333333333333) + ((count >> 2) & 0x3333333333333333);
  count = (count & 0x0f0f0f0f0f0f0f0f) + ((count >> 4) & 0x0f0f0f0f0f0f0f0f);
  count = (count & 0x00ff00ff00ff00ff) + ((count >> 8) & 0x00ff00ff00ff00ff);
  count = (count & 0x0000ffff0000ffff) + ((count >> 16) & 0x0000ffff0000ffff);
  return (int)((count & 0x00000000ffffffff) + ((count >> 32) & 0x00000000ffffffff));
}

// 整数を2進数表記で表すとき最低限必要なビット数を返す
inline int bit_length(int x) {
  for (int i = 31; i >= 0; --i) {
    if ((1 << i) & x) {
      return i + 1;
    }
  }
  return 1;
}

// 繰り返し二乗法による累乗
inline int pow(int a, int k) {
  if (k == 0) return 1;
  int res = pow(a, k / 2);
  res = res * res;
  if (k & 1) res = res * a;
  return res;
}

inline long long int pow(long long int a, long long int k) {
  if (k == 0) return 1;
  long long int res = pow(a, k / 2);
  res = res * res;
  if (k & 1) res = res * a;
  return res;
}

// newton法による平方根
inline int sqrt(int n) {
  if (n == 0) return 0;

  int x = pow(2, (bit_length(n) + 1) / 2);
  while (true) {
    int y = (x + n / x) / 2;
    if (y >= x) return x;
    x = y;
  }
}

// 平方数かどうかを判定する
template <typename T>
bool is_perfect_square(T x) {
  constexpr bool sq_mod256[] = {
      1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
      0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};

  if (!sq_mod256[x & 0xff]) return false;

  static const std::vector<char> mt[] = {{1, 1, 0, 0, 1, 0, 0, 1, 0},
                                         {1, 1, 0, 0, 1},
                                         {1, 1, 1, 0, 1, 0, 0},
                                         {1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1},
                                         {1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1}};

  const int a = x % (9 * 5 * 7 * 13 * 17);
  for (auto&& t : mt) {
    if (!t[a % t.size()]) return false;
  }

  return pow(sqrt(x), 2) == x;
}

template <typename T>
void log(T x) {
  std::cerr << x << std::endl;
}

template <typename T, typename... S>
void log(T x, S... rest) {
  std::cerr << x << " ";
  log(rest...);
}

}  // namespace kuroutil

#endif
