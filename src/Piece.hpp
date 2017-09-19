#ifndef PROCON_28_PIECE_HPP
#define PROCON_28_PIECE_HPP
#include "Geo.hpp"

#include <vector>

namespace procon28 {
class Piece {
public:
  long double area;
  Polygon poly;
  std::vector<Segment> segments;
  Piece();
  Piece(const Polygon&);
  Ring& outer();
  InnerContainer& inners();
  const Ring& outer() const;
  const InnerContainer& inners() const;

  friend std::istream& operator>>(std::istream& is, Piece& rhs) {
    is >> rhs.poly;

    const size_t N = rhs.poly.outer().size() - 1;
    rhs.segments.reserve(N);
    const Point& top = rhs.poly.outer().front();
    rhs.poly = translate(rhs.poly, -top.x(), -top.y());

    for (size_t i = 0; i < N; ++i) {
      rhs.segments.emplace_back(get_segment(rhs.poly, i));
    }
    rhs.area = bg::area(rhs.poly);

    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, Piece& rhs) {
    os << rhs.poly;
    return os;
  }
};

Piece translate(const Piece&, Point_Type, Point_Type);
Piece translate(const Piece&, Point);
Piece rotate(const Piece&, long double);
Piece rotate(const Piece&, long double, Point);
Piece inverse(const Piece&);

inline Ring& Piece::outer() {
  return poly.outer();
}

inline InnerContainer& Piece::inners() {
  return poly.inners();
}

inline const Ring& Piece::outer() const {
  return poly.outer();
}

inline const InnerContainer& Piece::inners() const {
  return poly.inners();
}

}  // namespace procon28

#endif
