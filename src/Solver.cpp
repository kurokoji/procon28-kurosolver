#include "Geo.hpp"
#include "KuroUtil.hpp"
#include "Piece.hpp"
#include "Solver.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

namespace procon28 {
Solver::Solver() {}
Solver::Solver(const std::vector<Piece>& pieces, const Polygon& frame)
    : pieces(pieces), frame(frame) {
      auto v = makeRotatePieces(pieces[0]);
      for (auto&& t : v) {
        std::cout << t;
      }
}

void Solver::SolvePruned() {

}

bool Solver::canRotate(Piece& piece, const long double angle, const Point& org) {
  piece = rotate(piece, angle, org);
  const int N = piece.outer().size() - 1;

  for (int i = 0; i < N; ++i) {
    const Point& p = piece.outer()[i];
    if (!eq(round(p.x()), p.x()) || !eq(round(p.y()), p.y())) {
      return false;
    }
  }

  for (auto&& p : piece.outer()) {
    p = Point(round(p.x()), round(p.y()));
  }

  return true;
}

// 合同かどうか
bool Solver::isCongruent(const Piece& lhs, const Piece& rhs) {
  if (lhs.outer().size() != rhs.outer().size()) return false;
  const size_t N = lhs.outer().size() - 1;
  for (size_t i = 0; i < N; ++i) {
    const Point p = rhs.outer().front() - lhs.outer()[i];
    Polygon g = translate(lhs.poly, p);
    bg::correct(g);
    if (bg::equals(g, rhs.poly)) return true;
  }

  return false;
}

std::vector<Piece> Solver::makeRotatePieces(const Piece& piece) {
  namespace util = kuroutil;
  std::vector<Piece> ret;
  ret.emplace_back(piece);

  const size_t segmentN = piece.segments.size();
  // 座標は左回り
  // 各辺に対して回転出来るやつを列挙する
  for (size_t i = 0; i < segmentN; ++i) {
    const Segment& s = piece.segments[i];
    const long double orgAngle = get_angle(s);
    const Point v = to_vec(s);
    const int len_sq = v.x() * v.x() + v.y() * v.y();
    const int MAX_X = std::floor(std::sqrt(len_sq));

    for (int x = 0; x <= MAX_X; ++x) {
      if (util::is_perfect_square(len_sq - x * x)) {
        const int y = util::sqrt(len_sq - x * x);
        long double nowAngle;
        if (v.x() <= 0) {
          nowAngle = std::atan2(y, x);
        } else {
          nowAngle = std::atan2(x, y);
        }
        // どれだけ回転するか(radianで与えられる)
        const double distAngle = orgAngle - nowAngle;
        Piece res = piece;
        if (canRotate(res, distAngle, piece.outer()[i])) {
          // 同じ図形かどうかを判定するために0番目の座標に合わせる
          const Point& fr = piece.outer().front();
          res = translate(res, -fr.x(), -fr.y());
          // すでに同じ図形があったらpushしない
          if (std::find_if(std::begin(ret), std::end(ret), [&](const Piece& x) {
                return isCongruent(res, x);
              }) == std::end(ret)) {
            ret.emplace_back(Piece(res.poly));
          }
        }
      }
    }
  }

  return ret;
}

};  // namespace procon28

#ifdef DEBUG
using namespace procon28;

int main() {
  Piece piece;
  std::cin >> piece;
  Polygon frame;
  std::vector<Piece> po;
  po.emplace_back(piece);
  Solver solver(po, frame);
}

#endif
