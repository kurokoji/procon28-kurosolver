#ifndef PROCON_28_SOLVER_HPP
#define PROCON_28_SOLVER_HPP

#include <iostream>
#include <vector>

#include "Geo.hpp"
#include "Piece.hpp"
#include "State.hpp"

namespace procon28 {
class Solver {
private:
  std::vector<Piece> pieces;
  std::vector<std::vector<Piece>> rotatePieces;
  Polygon frame;
  long double minimumAngle;
  bool canRotate(Piece&, const long double, const Point&);
  bool eq(long double, long double) const;
  bool isCongruent(const Piece&, const Piece&) const;
  long double round(long double) const;
  std::vector<Piece> makeRotatePieces(const Piece&);

  // ポインタを持っておいてあとから使ったピースなどを
  // 辿れるようにしておく
  class Context {
  public:
    State state;
    std::shared_ptr<Context> node;
    Context();
    Context(const State&, std::shared_ptr<Context>);
    Context(State&&, std::shared_ptr<Context>);
    void pieceAnswer(std::ostream&) const;
  };

public:
  Solver();
  Solver(const std::vector<Piece>&, const Polygon&);
  // 角度を合わせることを目標とするsolver
  void solveCorner(const State&) const;
  void solveSegment(const State&) const;
  void solveBeamSearch(const State&, const size_t) const;
  void solveChokudaiSearch(const State&, const size_t) const;
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
