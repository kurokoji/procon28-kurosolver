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

Point get_point(const Ring& ring, int n) {
  const size_t N = ring.size() - 1;
  return ring[n % N];
}

Point get_point(const Polygon& poly, int n) {
  return get_point(poly.outer(), n);
}

Segment get_segment(const Ring& ring, int a, int b) {
  return Segment(ring[a], ring[b]);
}

Segment get_segment(const Polygon& poly, int a, int b) {
  return get_segment(poly.outer(), a, b);
}

Segment get_segment(const Ring& ring, int n) {
  const size_t N = ring.size() - 1;
  return Segment(ring[n % N], ring[(n + 1) % N]);
}

Segment get_segment(const Polygon& poly, int n) {
  return get_segment(poly.outer(), n);
}
}  // namespace procon28
