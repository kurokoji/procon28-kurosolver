#include "Geo.hpp"
#include <vector>

using namespace procon28;

int main() {
  using std::cin;
  using std::cout;
  using std::endl;

  std::vector<Hole> holes;
  std::vector<Piece> pieces;
  int holes_N, pieces_N;

  cin >> holes_N;
  holes.reserve(holes_N);
  for (int i = 0; i < holes_N; ++i) {
    Hole hole;
    cin >> hole;
    holes.emplace_back(hole);
  }

  cin >> pieces_N;
  pieces.reserve(pieces_N);
  for (int i = 0; i < pieces_N; ++i) {
    Piece piece;
    cin >> piece;
    pieces.emplace_back(piece);
  }
}
