#ifndef PROCON_28_SOLVER_HPP
#define PROCON_28_SOLVER_HPP

#include <iostream>
#include <vector>
#include <queue>

#include "Geo.hpp"
#include "Piece.hpp"
#include "PostScript.hpp"
#include "State.hpp"
#include "KuroUtil.hpp"

namespace procon28 {
template <class T>
using Heap = std::priority_queue<T>;
template <class T>
using minHeap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

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

public:
  Solver();
  Solver(const std::vector<Piece>&, const Polygon&);
  // 角度を合わせることを目標とするsolver
  void solveCorner(const State&) const;
  void solveSegment(const State&) const;
  void solveSegMainSearch(const State&) const;
  void solveBeamSearch(const State&, const size_t) const;
  void solveChokudaiSearch(const State&, const size_t) const;

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

    template <typename... Types>
    void write(const char*, Types...) const;
    template <typename... Types>
    void write(orliv::misc::PostScript&, Types...) const;
  };
};

bool operator<(const std::shared_ptr<Solver::Context>&, const std::shared_ptr<Solver::Context>&);
bool operator>(const std::shared_ptr<Solver::Context>&, const std::shared_ptr<Solver::Context>&);

inline bool Solver::eq(long double lhs, long double rhs) const {
  return std::abs(lhs - rhs) <= EPS;
}

inline long double Solver::round(long double n) const {
  long double ret = std::round(n);
  if (ret == -0) ret = 0;
  return ret;
}


template <typename... Types>
void Solver::Context::write(const char* filename, Types... args) const {
  orliv::misc::PostScript ps{filename};
  write(ps, args...);
}

template <typename... Types>
void Solver::Context::write(orliv::misc::PostScript& ps, Types... args) const {
  auto ptr = this;

  ps.color(0, 0, 0);
  for (auto&& hole : ptr->state.frame.inners()) {
    ps.path(hole);
    ps.stroke();
  }

  while (ptr->node) {
    ps.random_color();
    ps.path(ptr->state.prev.outer());
    ps.fill();
    ptr = ptr->node.get();
  }

  ps.color(0, 0, 0);
  for (auto&& hole : ptr->state.frame.inners()) {
    ps.path(hole);
    ps.stroke();
  }

  ps.color(0, 0, 0);
  ps.path(ptr->state.frame.outer());
  ps.stroke();
  ps.text(10, 10, "[*]", args...);
  ps.render();
}
}  // namespace procon28

#endif
