#ifndef KUROUTIL_HPP
#define KUROUTIL_HPP

#include <iostream>
#include <vector>

namespace kuroutil {
// 立っているビット数を数える
template <typename T>
inline int popcount(T x) {
  unsigned int t = (unsigned int)(x);
  t = ((t & 0xAAAAAAAA) >> 1) + (t & 0x55555555);
  t = ((t & 0xCCCCCCCC) >> 2) + (t & 0x33333333);
  t = ((t & 0xF0F0F0F0) >> 4) + (t & 0x0F0F0F0F);
  t = ((t & 0xFF00FF00) >> 8) + (t & 0x00FF00FF);
  t = ((t & 0xFFFF0000) >> 16) + (t & 0x0000FFFF);

  return t;
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

}  // namespace kuroutil

#endif
