#include "Geo.hpp"
#include "Piece.hpp"
#include "State.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <tuple>
#include <vector>
#define BOOST_GEOMETRY_INCLUDE_SELF_TURNS

namespace procon28 {
State::State() {}
State::State(const Polygon& poly) : frame(poly), used(0), score(0) {
  bg::correct(frame);
}
State::State(Polygon&& poly) : frame(poly), used(0), score(0) {
  bg::correct(frame);
}
State::State(const Polygon& poly, const Polygon& pr, CheckType x, ScoreType score = 0)
    : frame(poly), prev(pr), used(x), score(score) {
  bg::correct(frame);
  for (auto&& p : pr.outer()) {
    pset.insert(std::make_pair((int)p.x(), (int)p.y()));
  }
}
State::State(Polygon&& poly, const Polygon& pr, CheckType x, ScoreType score = 0)
    : frame(poly), prev(pr), used(x), score(score) {
  bg::correct(frame);
  for (auto&& p : pr.outer()) {
    pset.insert(std::make_pair((int)p.x(), (int)p.y()));
  }
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
  if (nextFrame.inners().size() != 1) return false;
  for (auto&& hole : nextFrame.inners()) {
    for (size_t i = 0; i < hole.size() - 1; ++i) {
      auto&& cor = get_corner(hole, i);
      if (minimumAngle > cor && EPS < std::abs(cor - M_PI)) return false;
      // 1辺の長さが1cm(4グリッド幅)が保証されるためそれより小さいものは省く
      if (4.0 > bg::length(get_segment(hole, i))) return false;
    }
  }
  return true;
}

ScoreType State::convexHullEval(const Polygon& newFrame) const {
  ScoreType sum = 0.0;
  for (auto&& hole : newFrame.inners()) {
    Ring hull;
    bg::convex_hull(hole, hull);
    auto&& hulla = bg::area(hull);
    sum += hulla;
  }

  return sum;
}

ScoreType State::fitSegEval(const Polygon& piece, size_t holeN, int f, int p) const {
  const Ring& hole = frame.inners()[holeN];
  ScoreType sum = 0.0;
  Segment a = get_segment(hole, f), b = get_segment(piece, p), c = get_segment(hole, f - 1),
          d = get_segment(piece, p - 1);
  sum += bg::equals(a, b);
  sum += bg::equals(c, d);
  return sum;
}

ScoreType State::evaluation(const Polygon& nextFrame) const {
  return convexHullEval(nextFrame);
}

Polygon State::newFrame(const Polygon& piece) const {
  std::vector<Polygon> v;
  auto p = piece;
  bg::correct(p);
  bg::union_(frame, p, v);
  bg::correct(v[0]);

  return v[0];
}

// 角度を合わせて列挙する
std::vector<State> State::getNextCornerState(const std::vector<std::vector<Piece>>& rotatePieces,
                                             long double minimumAngle) const {
  std::vector<State> ret;

  using P = std::pair<int, int>;
  for (const Ring& hole : frame.inners()) {
    const size_t holepointN = hole.size() - 1;
    for (size_t h = 0; h < holepointN; ++h) {
      const size_t rotatePiecesN = rotatePieces.size();
      if (used && pset.find(P(hole[h].x(), hole[h].y())) == pset.end()) continue;
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

std::vector<State> State::getNextSegmentState(const std::vector<std::vector<Piece>>& rotatePieces,
                                              long double minimumAngle) const {
  std::vector<State> ret;

  using P = std::pair<int, int>;
  for (const Ring& hole : frame.inners()) {
    const size_t holepointN = hole.size() - 1;
    for (size_t h = 0; h < holepointN; ++h) {
      const size_t rotatePiecesN = rotatePieces.size();
      if (used && pset.find(P(hole[h].x(), hole[h].y())) == pset.end()) continue;
      for (size_t id = 0; id < rotatePiecesN; ++id) {
        if (used & (1ull << id)) continue;
        for (const Piece& piece : rotatePieces[id]) {
          const size_t pointN = piece.outer().size() - 1;
          for (size_t p = 0; p < pointN; ++p) {
            bool isFit;
            Polygon tr;
            std::tie(isFit, tr) = fitSegment(hole, h, piece.poly, p);
            if (!isFit) continue;
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

std::vector<State> State::getNextCornerPriSegState(const std::vector<std::vector<Piece>>& rotatePieces,
                                                   long double minimumAngle) const {
  std::vector<State> ret;

  using P = std::pair<int, int>;
  for (size_t i = 0; i < frame.inners().size(); ++i) {
    const Ring& hole = frame.inners()[i];
    const size_t holepointN = hole.size() - 1;
    for (size_t h = 0; h < holepointN; ++h) {
      const size_t rotatePiecesN = rotatePieces.size();
      if (used && pset.find(P(hole[h].x(), hole[h].y())) == pset.end()) continue;
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
            ret.emplace_back(nextFrame, tr, used | (1ull << id), fitSegEval(tr, i, h, p));
          }
        }
      }
    }
  }

  return ret;
}

}  // namespace procon28
