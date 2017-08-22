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
  Piece() {}
  Piece(Polygon& poly) : poly(poly) {
    const size_t N = poly.outer().size() - 1;
    segments.reserve(N);
    for (size_t i = 0; i < N; ++i) {
      segments.emplace_back(get_segment(poly, i));
    }
    area = bg::area(poly);
  }

  friend std::istream& operator>>(std::istream& is, Piece& rhs) {
    is >> rhs.poly;
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, Piece& rhs) {
    os << rhs.poly;
    return os;
  }
};

}  // namespace procon28

#endif
