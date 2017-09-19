#include "Geo.hpp"
#include "KuroUtil.hpp"
#include "Piece.hpp"
#include "State.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>

namespace procon28 {
State::State() {}
State::State(const Polygon& _frame) : frame(_frame), used(0) {}
State::State(const Polygon& _frame, const Polygon& _prev, CheckType x)
    : frame(_frame), prev(_prev), used(x) {}

std::tuple<bool, Polygon> State::fit(const Ring& hole, int a, const Polygon& piece, int b) const {
  long double ha = get_corner(hole, a);
  long double pa = get_corner(piece, b);
  Segment hs = get_segment(hole, a);
  Segment ps = get_segment(piece, b);
  // 角度が合ってなければだめ
  if (EPS < std::abs(get_angle(hs) - get_angle(ps))) return std::make_tuple(false, Polygon());
  if (EPS < std::abs(ha - pa)) return std::make_tuple(false, Polygon());
  Point hp = get_point(hole, a);
  Point pp = get_point(piece, b);
  return std::make_tuple(true, translate(piece, hp - pp));
}

bool State::canput(const Polygon& piece) const {
  std::vector<Polygon> res;
  bg::intersection(frame, piece, res);
  long double sum = 0.0;
  for (const Polygon& p : res) {
    sum += bg::area(p);
  }

  // かぶっている面積が今埋めようとしているピースの同じ面積ならOK
  return EPS >= std::abs(sum - bg::area(piece));
}

State State::nextState(const Polygon& piece, int id) const {
  std::vector<Polygon> merged;
  // フレームとピースの被っている部分を列挙
  bg::union_(frame, piece, merged);
  const size_t N = merged.size();
  // これあんま意味ないかも(意味なかったら外す)
  for (size_t i = 1; i < N; ++i) {
    merged.front().inners().emplace_back(merged[i].outer());
  }
  bg::correct(merged.front());
  // ピースをマージしたフレーム, 使ったピースの保存, 使ったピースのIDを記録
  return State(merged.front(), piece, used | (1ull << id));
}

// 角度を合わせて列挙する
std::vector<State> State::GetNextCornerState(
    const std::vector<std::vector<Piece>>& rotatePieces) const {
  namespace util = kuroutil;
  std::vector<State> ret;
  for (const Ring& hole : frame.inners()) {
    const size_t holepointN = hole.size() - 1;
    for (size_t h = 0; h < holepointN; ++h) {
      const size_t rotatePiecesN = rotatePieces.size();
      for (size_t id = 0; id < rotatePiecesN; ++id) {
        // すでに同一idのピースを使用していたら飛ばす
        if (used & (1ull << id)) continue;
        for (const Piece& piece : rotatePieces[id]) {
          const size_t pointN = piece.outer().size() - 1;
          for (size_t p = 0; p < pointN; ++p) {
            bool ok;
            Polygon tr;
            std::tie(ok, tr) = fit(hole, h, piece.poly, p);
            if (!ok) continue;
            if (!canput(tr)) continue;
            ret.emplace_back(nextState(tr, id));
          }
        }
      }
    }
  }
  return ret;
}
}  // namespace procon28
