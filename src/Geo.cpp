#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include "Geo.hpp"

namespace procon28 {
/* Polygonの入力 */
std::istream& operator>>(std::istream& is, Polygon& polygon) {
  size_t N;
  is >> N;
  // outer()はpolygonの外側の頂点集合を返す
  // inners()はpolygonの内側の頂点集合の集合(複数の穴)を返す
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

const Point operator+(const Point& lhs, const Point& rhs) {
  return Point(lhs.x() + rhs.x(), lhs.y() + rhs.y());
}

const Point operator-(const Point& lhs, const Point& rhs) {
  return Point(lhs.x() - rhs.x(), lhs.y() - rhs.y());
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

long double get_angle(const Segment& lhs, const Segment& rhs) {
  return std::abs(get_angle(rhs) - get_angle(lhs));
}

long double get_corner(const Ring& ring, int n) {
  const size_t N = ring.size() - 1;
  const size_t x = n % N;

  if (x == 0) {
    const Segment a = get_segment(ring, 0);
    const Segment b = get_segment(ring, N - 1);
    return get_angle(a, b);
  }
  const Segment a = get_segment(ring, x - 1);
  const Segment b = get_segment(ring, x);
  return get_angle(a, b);
}

long double get_corner(const Polygon& poly, int n) {
  return get_corner(poly.outer(), n);
}

Point to_vec(const Segment& s) {
  return Point(s.second.x() - s.first.x(), s.second.y() - s.first.y());
}

Polygon to_frame(const std::vector<Polygon>& holes) {
  bg::model::box<Point> box(Point(-500, -500), Point(500, 500));
  Polygon ret;
  bg::convert(box, ret);

  for (auto&& hole : holes) {
    ret.inners().emplace_back(hole.outer());
  }
  bg::correct(ret);
  return ret;
}

std::vector<Polygon> to_piece(const Polygon& frame) {
  std::vector<Polygon> ret;
  ret.reserve(frame.inners().size());
  for (auto&& hole : frame.inners()) {
    Polygon poly;
    for (auto&& p : hole) {
      poly.outer().emplace_back(p);
    }
    ret.emplace_back(poly);
  }
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

Polygon inverse(const Polygon& poly) {
  Polygon ret = poly;
  for (auto& p : ret.outer()) p = Point(-p.x(), p.y());
  return ret;
}

Polygon scale(const Polygon& p, double s) {
  auto origin = bg::return_centroid<Point>(p);
  trans::scale_transformer<long double, 2, 2> tr(s);
  Polygon r;
  bg::transform(translate(p, -origin.x(), -origin.y()), r, tr);
  return translate(r, origin.x(), origin.y());
}

void printFrame(const Polygon& frame, std::ostream& os) {
  os << frame.inners().size() << '\n';
  for (auto&& hole : frame.inners()) {
    os << hole.size() << '\n';
    for (auto&& p : hole) {
      os << p.x() << " " << p.y() << '\n';
    }
  }
  os << std::flush;
}
}  // namespace procon28
