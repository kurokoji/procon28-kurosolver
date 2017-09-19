#ifndef PROCON_28_SOLVER_HPP
#define PROCON_28_SOLVER_HPP

#include <iostream>
#include <vector>

#include "Geo.hpp"
#include "Piece.hpp"

namespace procon28 {
class Solver {
private:
  std::vector<Piece> pieces;
  std::vector<std::vector<Piece>> rotatePieces;
  Polygon frame;
  bool canRotate(Piece&, const long double, const Point&);
  bool eq(long double, long double) const;
  bool isCongruent(const Piece&, const Piece&) const;
  long double round(long double) const;
  std::vector<Piece> makeRotatePieces(const Piece&);
public:
  Solver();
  Solver(const std::vector<Piece>&, const Polygon&);
  // 角度を合わせることを目標とするsolver
  void SolveCorner();
};

inline bool Solver::eq(long double lhs, long double rhs) const {
  return std::abs(lhs - rhs) <= EPS;
}

inline long double Solver::round(long double n) const {
  long double ret = std::round(n);
  if (ret == -0) ret = 0;
  return ret;
}
}  // namespace procon28

#endif
