#include "Geo.hpp"
#include "Piece.hpp"

#include <vector>

namespace procon28 {
Piece::Piece() {}
Piece::Piece(const Polygon& _poly) : poly(_poly) {
  const size_t N = poly.outer().size() - 1;
  segments.reserve(N);

  // 0番目のPointを基準(原点とする)
  const Point& top = poly.outer().front();
  poly = translate(poly, -top.x(), -top.y());

  for (size_t i = 0; i < N; ++i) {
    segments.emplace_back(get_segment(poly, i));
  }
  area = bg::area(poly);
}

Piece translate(const Piece& piece, Point_Type x, Point_Type y) {
  return Piece(translate(piece.poly, x, y));
}

Piece translate(const Piece& piece, Point p) {
  return translate(piece, p.x(), p.y());
}

Piece rotate(const Piece& piece, long double ang) {
  return Piece(rotate(piece.poly, ang));
}

Piece rotate(const Piece& piece, long double ang, Point org) {
  return rotate(translate(piece, -org.x(), -org.y()), ang);
}
Piece inverse(const Piece& piece) {
  return Piece(inverse(piece.poly));
}
}  // namespace procon28
