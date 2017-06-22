#include "Geo.hpp"
#include "Piece.hpp"
#include "Hole.hpp"
#include <vector>

using namespace procon28;

int main() {
  using std::cin;
  using std::cout;
  using std::endl;

  // cin高速化
  std::cin.tie(0);
  std::ios::sync_with_stdio(false);

  std::vector<Hole> holes;
  std::vector<Piece> pieces;
  int holes_N, pieces_N;

  cin >> pieces_N;
  pieces.reserve(pieces_N);
  for (int i = 0; i < pieces_N; ++i) {
    Piece piece;
    cin >> piece;
    pieces.emplace_back(piece);
  }

  cin >> holes_N;
  holes.reserve(holes_N);
  for (int i = 0; i < holes_N; ++i) {
    Hole hole;
    cin >> hole;
    holes.emplace_back(hole);
  }


  cout << pieces_N << endl;
  for (auto&& piece : pieces) cout << piece;
  cout << holes_N << endl;
  for (auto&& hole : holes) cout << hole;
}
