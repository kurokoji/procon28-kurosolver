#ifndef PROCON_28_SOLVER_HPP
#define PROCON_28_SOLVER_HPP

#include <iostream>
#include <vector>

#include "Geo.hpp"
#include "Hole.hpp"
#include "Piece.hpp"

namespace procon28 {
class Solver {
private:
  std::vector<Piece> pieces;
  std::vector<Hole> holes;

public:
  Solver() {}
  Solver(std::vector<Piece>& pieces, std::vector<Hole>& holes) : pieces(pieces), holes(holes) {}
};
}  // namespace procon28

#endif
