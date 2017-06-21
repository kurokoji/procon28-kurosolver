#include "Geo.hpp"

namespace procon28 {
/* Polygonの入力 */
std::istream& operator>>(std::istream& is, Polygon& polygon) {
  int N;
  is >> N;
  polygon.outer().clear();
  polygon.outer().reserve(N);
  for (int i = 0; i < N; ++i) {
    long double x, y;
    is >> x >> y;
    polygon.outer().emplace_back(x, y);
  }

  bg::correct(polygon);
  return is;
}
/* Polygonの出力 */
std::ostream& operator<<(std::ostream& os, Polygon& polygon) {
  int N = polygon.outer().size() - 1;
  os << N << '\n';

  for (int i = 0; i < N; ++i) {
    os << polygon.outer()[i].x() << " " << polygon.outer()[i].y() << '\n';
  }
  os << std::flush;

  return os;
}

} // namespace procon28
