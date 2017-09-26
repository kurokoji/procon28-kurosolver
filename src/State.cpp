#include "Geo.hpp"
#include "Piece.hpp"
#include "State.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>
#define BOOST_GEOMETRY_INCLUDE_SELF_TURNS

namespace procon28 {
State::State() {}
State::State(const Polygon& poly) : frame(poly), used(0) {
  bg::correct(frame);
}
State::State(Polygon&& poly) : frame(poly), used(0) {
  bg::correct(frame);
}
State::State(const Polygon& poly, const Polygon& pr, CheckType x, ScoreType score = -1) : frame(poly), prev(pr), used(x), score(score) {
  bg::correct(frame);
}
State::State(Polygon&& poly, const Polygon& pr, CheckType x, ScoreType score = -1) : frame(poly), prev(pr), used(x), score(score) {
  bg::correct(frame);
}

std::tuple<bool, Polygon> State::fitCorner(const Ring& hole, int a, const Polygon& piece, int b) const {
  long double ha = get_corner(hole, a);
  long double pa = get_corner(piece, b);
  Segment&& hs = get_segment(hole, a);
  Segment&& ps = get_segment(piece, b);
  // 角度が合ってなければだめ
  if (EPS < std::abs(get_angle(hs) - get_angle(ps))) return std::make_tuple(false, Polygon());
  if (EPS < std::abs(ha - pa)) return std::make_tuple(false, Polygon());
  Point&& hp = get_point(hole, a);
  Point&& pp = get_point(piece, b);
  return std::make_tuple(true, translate(piece, hp - pp));
}

std::tuple<bool, Polygon> State::fitSegment(const Ring& hole, int a, const Polygon& piece, int b) const {
  Segment&& hs = get_segment(hole, a);
  Segment&& ps = get_segment(piece, b);
  if (EPS < std::abs(get_angle(hs) - get_angle(ps))) return std::make_tuple(false, Polygon());
  long double hslen = bg::length(hs);
  long double pslen = bg::length(ps);
  if (EPS < std::abs(hslen - pslen)) return std::make_tuple(false, Polygon());
  Point&& hp = get_point(hole, a);
  Point&& pp = get_point(piece, b);
  return std::make_tuple(true, translate(piece, hp - pp));
}

bool State::canPut(const Polygon& piece) const {
  auto p = piece;
  bg::correct(p);
  std::vector<Polygon> v;
  bg::intersection(frame, p, v);
  return v.size() == 0;
}

bool State::canUseFrame(const Polygon& nextFrame, long double minimumAngle) const {
  for (auto&& hole : nextFrame.inners()) {
    for (size_t i = 0; i < hole.size() - 1; ++i) {
      if (minimumAngle > get_corner(hole, i)) return false;
      // 1辺の長さが1cm(4グリッド幅)が保証されるためそれより小さいものは省く
      if (4.0 > bg::length(get_segment(hole, i))) return false;
    }
  }
  return true;
}

ScoreType State::evaluation(const Polygon& nextFrame) const {
  ScoreType sum = 0.0;
  for (auto&& hole : nextFrame.inners()) {
    Ring hull;
    bg::convex_hull(hole, hull);
    auto&& hulla = bg::area(hull);
    sum += hulla;
  }

  return sum;
}

Polygon State::newFrame(const Polygon& _piece) const {
  std::vector<Polygon> v;
  auto p = _piece;
  bg::correct(p);
  bg::union_(frame, p, v);
  bg::correct(v[0]);

  return v[0];
}

// 角度を合わせて列挙する
std::vector<State> State::getNextCornerState(const std::vector<std::vector<Piece>>& rotatePieces,
                                             long double minimumAngle) const {
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
            std::tie(ok, tr) = fitCorner(hole, h, piece.poly, p);
            if (!ok) continue;
            if (!canPut(tr)) continue;
            Polygon nextFrame = newFrame(tr);
            if (!canUseFrame(nextFrame, minimumAngle)) continue;
            ret.emplace_back(nextFrame, tr, used | (1ull << id), evaluation(nextFrame));
          }
        }
      }
    }
  }
  return ret;
}
}  // namespace procon28
