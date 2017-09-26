#include "Geo.hpp"
#include "Piece.hpp"
#include "Solver.hpp"
#include "State.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace procon28;

int main(int argc, char *argv[]) {
  if (argc == 0) return -1;

  using std::cin;
  using std::cout;
  using std::endl;

  // cin高速化
  std::cin.tie(0);
  std::ios::sync_with_stdio(false);

  std::vector<Polygon> holes;
  std::vector<Piece> pieces;

  size_t holes_N, pieces_N;

  cin >> pieces_N;
  pieces.reserve(pieces_N);
  for (size_t i = 0; i < pieces_N; ++i) {
    Piece piece;
    cin >> piece;
    pieces.emplace_back(piece);
  }

  cin >> holes_N;
  holes.reserve(holes_N);
  for (size_t i = 0; i < holes_N; ++i) {
    Polygon hole;
    cin >> hole;
    holes.emplace_back(hole);
  }
  Polygon frame = to_frame(holes);

  std::sort(std::begin(pieces), std::end(pieces), [&](const Piece& lhs, const Piece& rhs) {
    return lhs.area < rhs.area;
  });
  State init(frame);
  Solver solver(pieces, frame);
  // solver.solveCorner(init);
  solver.solveBeamSearch(init, std::atoi(argv[1]));
}
