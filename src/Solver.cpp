#include "Geo.hpp"
#include "KuroUtil.hpp"
#include "Piece.hpp"
#include "PostScript.hpp"
#include "Solver.hpp"
#include "State.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

namespace procon28 {
bool operator<(const std::shared_ptr<Solver::Context>& lhs, const std::shared_ptr<Solver::Context>& rhs) {
  return lhs->state.score < rhs->state.score;
}

bool operator>(const std::shared_ptr<Solver::Context>& lhs, const std::shared_ptr<Solver::Context>& rhs) {
  return lhs->state.score > rhs->state.score;
}

Solver::Context::Context() {}
Solver::Context::Context(const State& s, std::shared_ptr<Context> ptr = nullptr) : state(s), node(ptr) {}
Solver::Context::Context(State&& s, std::shared_ptr<Context> ptr = nullptr) : state(s), node(ptr) {}
void Solver::Context::pieceAnswer(std::ostream& os) const {
  std::vector<Polygon> res;
  for (auto ptr = this; ptr->node != nullptr; ptr = ptr->node.get()) {
    res.emplace_back(ptr->state.prev);
  }
  os << res.size() << std::endl;
  for (auto&& p : res) {
    os << p;
  }
}

Solver::Solver() {}
Solver::Solver(const std::vector<Piece>& pieces, const Polygon& frame) : pieces(pieces), frame(frame) {
  minimumAngle = PI * 2;
  for (auto&& piece : pieces) {
    rotatePieces.emplace_back(makeRotatePieces(piece));
    for (size_t i = 0; i < piece.outer().size() - 1; ++i) {
      minimumAngle = std::min(minimumAngle, get_corner(piece.poly, i));
    }
  }
  orliv::misc::PostScript rotatepiece("rotate.ps");
  for (size_t i = 0; i < rotatePieces.size(); ++i) {
    rotatepiece.random_color();
    for (auto&& piece : rotatePieces[i]) {
      auto po = translate(piece.poly, 50, 50);
      rotatepiece.path(po.outer());
      rotatepiece.fill();
      rotatepiece.color(0, 0, 0);
      rotatepiece.text(10, 10, po.outer().size()," ", i);
      rotatepiece.render();
    }
  }
  std::cerr << minimumAngle << std::endl;
}

void Solver::solveCorner(const State& ini) const {
  namespace util = kuroutil;
  using context_ptr = std::shared_ptr<Context>;
  const size_t N = pieces.size();

  orliv::misc::PostScript dbg("dbg.ps");
  std::stack<context_ptr> st;
  auto best = std::make_shared<Context>(ini);
  st.emplace(best);
  size_t ma = 0;
  auto start_time = std::chrono::system_clock::now();
  util::log("DFS start!!", N);
  while (!st.empty()) {
    auto v = st.top();
    st.pop();
    const size_t cnt = util::popcount(v->state.used);
    if (ma < cnt) {
      best = v;
      ma = cnt;
      util::log("put", cnt, st.size());
      best->write(dbg, cnt);
    }

    if (cnt == N) break;
    auto&& nextState = v->state.getNextCornerState(rotatePieces, minimumAngle);
    for (auto&& sta : nextState) {
      st.emplace(std::make_shared<Context>(sta, v));
    }
  }
  auto end_time = std::chrono::system_clock::now();
  auto diff = end_time - start_time;
  util::log("minutes:", std::chrono::duration_cast<std::chrono::minutes>(diff).count());
  util::log("seconds:", std::chrono::duration_cast<std::chrono::seconds>(diff).count());

  best->pieceAnswer(std::cout);
  printFrame(frame, std::cout);
}

void Solver::solveSegment(const State& ini) const {
  namespace util = kuroutil;
  using context_ptr = std::shared_ptr<Context>;
  const size_t N = pieces.size();

  orliv::misc::PostScript dbg("dbg.ps");
  std::stack<context_ptr> st;
  auto best = std::make_shared<Context>(ini);
  st.emplace(best);
  size_t ma = 0;
  auto start_time = std::chrono::system_clock::now();
  util::log("DFS start!!", N);
  while (!st.empty()) {
    auto v = st.top();
    st.pop();
    const size_t cnt = util::popcount(v->state.used);
    if (ma < cnt) {
      best = v;
      ma = cnt;
      util::log("put", cnt, st.size());
      best->write(dbg, cnt);
    }

    if (cnt == N) break;
    auto&& nextState = v->state.getNextSegmentState(rotatePieces, minimumAngle);
    for (auto&& sta : nextState) {
      st.emplace(std::make_shared<Context>(sta, v));
    }
  }
  auto end_time = std::chrono::system_clock::now();
  auto diff = end_time - start_time;
  util::log("minutes:", std::chrono::duration_cast<std::chrono::minutes>(diff).count());
  util::log("seconds:", std::chrono::duration_cast<std::chrono::seconds>(diff).count());

  best->pieceAnswer(std::cout);
  printFrame(frame, std::cout);
}

void Solver::solveBeamSearch(const State& ini, const size_t beamWidth) const {
  namespace util = kuroutil;
  using context_ptr = std::shared_ptr<Context>;
  const size_t N = pieces.size();
  orliv::misc::PostScript("dbg.ps");
  std::vector<context_ptr> beam, next;
  auto best = std::make_shared<Context>(ini);

  beam.emplace_back(best);

  orliv::misc::PostScript dbg("dbg.ps");
  util::log("BeamSearch Start!!", N);
  auto start_time = std::chrono::system_clock::now();
  while (!beam.empty()) {
    best = beam.front();
    const size_t cnt = util::popcount(best->state.used);
    util::log("Step", cnt, "Size", beam.size());
    best->write(dbg, cnt);
    if (cnt == N) break;
    std::vector<context_ptr> can;
    for (auto&& con : beam) {
      auto&& nextCornerState = con->state.getNextCornerPriSegState(rotatePieces, minimumAngle);
      for (auto&& sta : nextCornerState) {
        can.emplace_back(std::make_shared<Context>(sta, con));
      }
      /*
      auto&& nextSegmentState = con->state.getNextSegmentState(rotatePieces, minimumAngle);
      for (auto&& sta : nextSegmentState) {
        can.emplace_back(std::make_shared<Context>(sta, con));
      }
      */
    }
    util::log(can.size());

    const int wid = std::min(beamWidth, can.size());
    std::partial_sort(std::begin(can), std::begin(can) + wid, std::end(can), std::greater<context_ptr>());
    next.clear();
    for (size_t i = 0; i < wid; ++i) {
      next.emplace_back(can[i]);
    }
    beam.swap(next);
  }
  auto end_time = std::chrono::system_clock::now();
  auto diff = end_time - start_time;
  util::log("minutes:", std::chrono::duration_cast<std::chrono::minutes>(diff).count());
  util::log("seconds:", std::chrono::duration_cast<std::chrono::seconds>(diff).count());
  best->pieceAnswer(std::cout);
  printFrame(frame, std::cout);
}

void Solver::solveChokudaiSearch(const State& ini, const size_t maxTime) const {
  namespace util = kuroutil;
  using context_ptr = std::shared_ptr<Context>;
  const size_t N = pieces.size();

  std::vector<Heap<context_ptr>> heap(N + 1);
  auto best = std::make_shared<Context>(ini);
  heap[0].emplace(best);
  size_t ma = 0;
  orliv::misc::PostScript dbg("dbg.ps");
  util::TimeCheck timeManage(maxTime);
  size_t chokudaiWitdh = 1;
  util::log("ChokudaiSearch Start!!", N);
  while (!timeManage.isFinish()) {
    for (size_t t = 0; t < N; ++t) {
      for (size_t i = 0; i < chokudaiWitdh; ++i) {
        if (heap[t].empty()) break;
        auto v = heap[t].top();
        heap[t].pop();
        if (t + 1 > ma) {
          ma = t + 1;
          best = v;
          best->write(dbg, t + 1);
          util::log("Step...", t + 1);
          if (ma >= N) goto END;
        }
        auto&& nextCornerState = v->state.getNextCornerPriSegState(rotatePieces, minimumAngle);
        for (auto&& sta : nextCornerState) {
          heap[t + 1].emplace(std::make_shared<Context>(sta, v));
        }
      }
    }
  }

END:
  best->write(dbg, "po");
  best->pieceAnswer(std::cout);
  printFrame(frame, std::cout);
}

void Solver::solveSegMainSearch(const State& ini) const {
  namespace util = kuroutil;
  using context_ptr = std::shared_ptr<Context>;
  const size_t N = pieces.size();

  orliv::misc::PostScript dbg("dbg.ps");
  std::stack<context_ptr> st;
  auto best = std::make_shared<Context>(ini);
  st.emplace(best);
  size_t ma = 1;
  auto start_time = std::chrono::system_clock::now();
  {
    auto v = st.top();
    st.pop();
    const size_t cnt = util::popcount(v->state.used);
    best = v;
    util::log("put", cnt, st.size());
    best->write(dbg, cnt);

    auto&& nextState = v->state.getNextCornerState(rotatePieces, minimumAngle);
    for (auto&& sta : nextState) {
      st.emplace(std::make_shared<Context>(sta, v));
    }
  }
  util::log("DFS start!!", N);
  while (!st.empty()) {
    auto v = st.top();
    st.pop();
    const size_t cnt = util::popcount(v->state.used);
    if (ma < cnt) {
      best = v;
      ma = cnt;
      util::log("put", cnt, st.size());
      best->write(dbg, cnt);
    }

    if (cnt == N) break;
    auto&& nextState = v->state.getNextSegmentState(rotatePieces, minimumAngle);
    for (auto&& sta : nextState) {
      st.emplace(std::make_shared<Context>(sta, v));
    }
  }
  auto end_time = std::chrono::system_clock::now();
  auto diff = end_time - start_time;
  util::log("minutes:", std::chrono::duration_cast<std::chrono::minutes>(diff).count());
  util::log("seconds:", std::chrono::duration_cast<std::chrono::seconds>(diff).count());

  best->pieceAnswer(std::cout);
  printFrame(frame, std::cout);
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
bool Solver::isCongruent(const Piece& lhs, const Piece& rhs) const {
  if (lhs.outer().size() != rhs.outer().size()) return false;
  auto r = rhs.poly;
  bg::correct(r);
  const size_t N = lhs.outer().size() - 1;
  for (size_t i = 0; i < N; ++i) {
    const Point p = rhs.outer().front() - lhs.outer()[i];
    Polygon g = translate(lhs.poly, p);
    bg::correct(g);
    if (bg::equals(g, r)) return true;
  }

  return false;
}

std::vector<Piece> Solver::makeRotatePieces(const Piece& piece) {
  namespace util = kuroutil;
  std::vector<Piece> ret;
  ret.emplace_back(piece);

  const size_t segmentN = piece.segments.size();
  // 座標は時計回り
  // 各辺に対して回転出来るやつを列挙する
  for (size_t i = 0; i < segmentN; ++i) {
    const Segment& s = piece.segments[i];
    const long double orgAngle = get_angle(s);
    const Point v = to_vec(s);
    const int len_sq = v.x() * v.x() + v.y() * v.y();
    const int MAX_X = std::floor(std::sqrt(len_sq));

    for (int x = 0; x <= MAX_X; ++x) {
      // 平方数であれば使える
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
        if (canRotate(res, distAngle, res.outer()[i])) {
          // 同じ図形かどうかを判定するために0番目の座標に合わせる
          const Point& fr = res.outer().front();
          res = translate(res, -fr.x(), -fr.y());
          // すでに同じ図形があったらpushしない
          if (std::find_if(std::begin(ret), std::end(ret), [&](const Piece& x) { return isCongruent(res, x); }) ==
              std::end(ret)) {
            ret.emplace_back(Piece(res.poly));
          }
        }

        Piece resInversed = inverse(piece);

        if (canRotate(resInversed, distAngle, resInversed.outer()[i])) {
          // 同じ図形かどうかを判定するために0番目の座標に合わせる
          const Point& fr = resInversed.outer().front();
          resInversed = translate(resInversed, -fr.x(), -fr.y());
          // すでに同じ図形があったらpushしない
          if (std::find_if(std::begin(ret), std::end(ret),
                           [&](const Piece& x) { return isCongruent(resInversed, x); }) == std::end(ret)) {
            ret.emplace_back(Piece(resInversed.poly));
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
