#ifndef PROCON_28_STATE_HPP
#define PROCON_28_STATE_HPP

#include "Geo.hpp"
#include "Piece.hpp"

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <map>

namespace procon28 {
using CheckType = unsigned long long int;
using ScoreType = long double;
class State {
private:
  std::set<std::pair<int, int>> pset;
  std::tuple<bool, Polygon> fitCorner(const Ring&, int, const Polygon&, int) const;
  std::tuple<bool, Polygon> fitSegment(const Ring&, int, const Polygon&, int) const;
  bool canPut(const Polygon&) const;
  bool canUseFrame(const Polygon&, long double) const;
  Polygon newFrame(const Polygon&) const;
  ScoreType convexHullEval(const Polygon&) const;
  ScoreType fitSegEval(const Polygon&, size_t, int, int) const;
  ScoreType evaluation(const Polygon&) const;

public:
  // 最大ピース数は50なので64bitで十分足りる
  Polygon frame, prev;
  CheckType used;
  ScoreType score;
  State();
  State(const Polygon&);
  State(Polygon&&);
  State(const Polygon&, const Polygon&, CheckType, ScoreType);
  State(Polygon&&, const Polygon&, CheckType, ScoreType);
  std::vector<State> getNextCornerState(const std::vector<std::vector<Piece>>&, long double) const;
  std::vector<State> getNextSegmentState(const std::vector<std::vector<Piece>>&, long double) const;
  std::vector<State> getNextCornerPriSegState(const std::vector<std::vector<Piece>>&, long double) const;
};
}  // namespace procon28

#endif
