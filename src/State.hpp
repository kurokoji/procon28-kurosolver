#ifndef PROCON_28_STATE_HPP
#define PROCON_28_STATE_HPP

#include "Geo.hpp"
#include "Piece.hpp"

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

namespace procon28 {
using CheckType = unsigned long long int;
using ScoreType = long double;
class State {
private:
  std::tuple<bool, Polygon> fitCorner(const Ring&, int, const Polygon&, int) const;
  bool canPut(const Polygon&) const;
  bool canUseFrame(const Polygon&, long double) const;
  Polygon newFrame(const Polygon&) const;
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
};
}  // namespace procon28

#endif
