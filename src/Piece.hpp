#pragma once
#ifndef PROCON_28_PIECE_HPP
#define PROCON_28_PIECE_HPP
#include "Geo.hpp"

#include <vector>

namespace procon28 {
class Piece {
public:
  double area;
  Polygon poly;
  Piece() {}
  Piece(Polygon& poly) : poly(poly) {}

  friend std::istream& operator>>(std::istream& is, Piece& rhs) {
    is >> rhs.poly;
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, Piece& rhs) {
    os << rhs.poly;
    return os;
  }
};

} // namespace procon28

#endif
