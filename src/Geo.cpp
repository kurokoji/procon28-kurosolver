#include "Geo.hpp"

namespace procon28 {
/* Polygonの入力 */
std::istream& operator>>(std::istream& is, Polygon& polygon) {
  size_t N;
  is >> N;
  // outer()はpolygonの外側の頂点集合を返す
  // inner()はpolygonの内側の頂点集合を返す
  polygon.outer().clear();
  polygon.outer().reserve(N);
  for (size_t i = 0; i < N; ++i) {
    Point_Type x, y;
    is >> x >> y;
    polygon.outer().emplace_back(x, y);
  }

  // 足りない頂点とかをいい感じに追加してくれる
  bg::correct(polygon);
  return is;
}
/* Polygonの出力 */
std::ostream& operator<<(std::ostream& os, Polygon& polygon) {
  const size_t N = polygon.outer().size() - 1;
  os << N << '\n';

  for (size_t i = 0; i < N; ++i) {
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

long double get_angle(const Segment& s) {
  Point v = to_vec(s);
  return atan2(v.y(), v.x());
}

Point to_vec(const Segment& s) {
  return Point(s.second.x() - s.first.x(), s.second.y() - s.first.y());
}

Polygon to_frame(const std::vector<Polygon>& holes) {
  bg::model::box<Point> box(Point(-20, -20), Point(120, 120));
  Polygon ret;
  bg::convert(box, ret);

  for (auto&& hole : holes) {
    ret.inners().emplace_back(hole.outer());
  }
  bg::correct(ret);
  return ret;
}

Polygon translate(const Polygon& poly, Point_Type x, Point_Type y) {
  trans::translate_transformer<Point_Type, 2, 2> tr(x, y);
  Polygon ret;
  bg::transform(poly, ret, tr);
  return ret;
}

Polygon translate(const Polygon& poly, Point p) {
  return translate(poly, p.x(), p.y());
}

Polygon rotate(const Polygon& poly, long double ang) {
  trans::rotate_transformer<bg::radian, Point_Type, 2, 2> rotate(ang);
  Polygon ret;
  bg::transform(poly, ret, rotate);
  return ret;
}

Polygon rotate(const Polygon& poly, long double ang, Point org) {
  return rotate(translate(poly, -org.x(), -org.y()), ang);
}
}  // namespace procon28
