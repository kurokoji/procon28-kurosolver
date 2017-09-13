#ifndef PROCON_28_SOLVER_HPP
#define PROCON_28_SOLVER_HPP

#include <iostream>
#include <vector>

#include "Geo.hpp"
#include "Piece.hpp"

namespace procon28 {
const long double EPS = 1e-10;
class Solver {
private:
  std::vector<Piece> pieces;
  std::vector<std::vector<Piece>> rotatePieces;
  Polygon frame;
  bool canRotate(Piece&, const long double, const Point&);
  bool eq(long double, long double);
  bool isCongruent(const Piece&, const Piece&);
  long double round(long double);
  std::vector<Piece> makeRotatePieces(const Piece&);
public:
  Solver();
  Solver(const std::vector<Piece>&, const Polygon&);
  // 枝刈り全探索による解法
  void SolvePruned();
};

inline bool Solver::eq(long double lhs, long double rhs) {
  return std::abs(lhs - rhs) <= EPS;
}

inline long double Solver::round(long double n) {
  long double ret = std::round(n);
  if (ret == -0) ret = 0;
  return ret;
}
}  // namespace procon28

#endif
