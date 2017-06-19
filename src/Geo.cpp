#include "Geo.hpp"

namespace procon28 {
/* Polygonの入力 */
std::istream &operator>>(std::istream &is, Polygon &polygon) {
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
  int N = polygon.outer().size();
  os << N << '\n';
  for (auto&& point : polygon.outer()) {
    os << point.x() << " " << point.y() << '\n';
  }

  std::flush();
}

} // namespace procon28
