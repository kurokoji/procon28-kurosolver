#ifndef PROCON_28_STATE_HPP
#define PROCON_28_STATE_HPP

#include "Geo.hpp"
#include "Piece.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

namespace procon28 {
using CheckType = unsigned long long int;
class State {
private:
  Polygon frame, prev;
  // 最大ピース数は50なので64bitで十分足りる
  CheckType used;
  std::tuple<bool, Polygon> fit(const Ring&, int, const Polygon&, int) const;
  bool canput(const Polygon&) const;
  State nextState(const Polygon&, int) const;

public:
  State();
  State(const Polygon&);
  State(const Polygon&, const Polygon&, CheckType);
  std::vector<State> GetNextCornerState(const std::vector<std::vector<Piece>>&) const;
};
}  // namespace procon28

#endif
