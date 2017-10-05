// {{{  orliv::misc::PostScript
#ifndef INCLUDE_POST_SCRIPT_HPP
#define INCLUDE_POST_SCRIPT_HPP
#include <algorithm>
#include <cassert>
#include <complex>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace orliv {
namespace misc {

template <typename T>
struct PointTraits {
  using value_type = double;
  static value_type x(const T& p) { return p.x(); }
  static value_type y(const T& p) { return p.y(); }
};

template <>
struct PointTraits<std::complex<double>> {
  using value_type = double;
  static value_type x(const std::complex<double>& p) { return p.real(); }
  static value_type y(const std::complex<double>& p) { return p.imag(); }
};

struct PostScript {
  PostScript(const std::string& filename, int rux = 1000, int ruy = 1000, int ldx = 0, int ldy = 0)
      : ofs(filename) {
    ofs << "%!PS-Adobe-3.0 EPSF-3.0\n";
    ofs << "%%BoundingBox: 0 0 " << (rux - ldx) << ' ' << (ruy - ldy) << '\n';
    offset(-ldx, -ldy);
    //ofs << "/Times-Roman findfont 22 scalefont setfont\n";
    ofs << "/Times-Roman findfont 12 scalefont setfont\n";    
    ofs << "gsave\n";
  }
  void offset(double x, double y) { ofs << x << ' ' << y << " translate\n"; }
  void render() { ofs << "showpage grestore gsave\n"; }
  void color(double r, double g, double b) { ofs << r << ' ' << g << ' ' << b << " setrgbcolor\n"; }
  void random_color() { color(col(rng), col(rng), col(rng)); }
  void gray(double n) { ofs << n << " setgray\n"; }
  void line(double x1, double y1, double x2, double y2) {
    moveTo(x1, y1);
    lineTo(x2, y2);
    stroke();
  }
  void rect(double x, double y, double w, double h, bool fill = false) {
    path({{x, y}, {x + w, y}, {x + w, y + h}, {x, y + h}});
    fillOrStroke(fill);
  }
  void triangle(double x1, double y1, double x2, double y2, double x3, double y3,
                bool fill = false) {
    path({{x1, y1}, {x2, y2}, {x3, y3}});
    fillOrStroke(fill);
  }
  void circle(double x, double y, double r, bool fill = false) {
    ofs << x << ' ' << y << ' ' << r << " 0 360 arc\n";
    fillOrStroke(fill);
  }
  template <typename T>
  void path(const std::vector<T>& points) {
    assert(points.size() > 2);
    moveTo(PointTraits<T>::x(points[0]), PointTraits<T>::y(points[0]));
    for (int i = 1; i < points.size(); i++) {
      lineTo(PointTraits<T>::x(points[i]), PointTraits<T>::y(points[i]));
    }
    ofs << "closepath\n";
  }
  void path(const std::initializer_list<std::initializer_list<double>>& points) {
    std::vector<std::complex<double>> ps;
    ps.reserve(points.size());
    for (const auto& p : points) {
      assert(p.size() == 2);
      ps.emplace_back(p.begin()[0], p.begin()[1]);
    }
    path(ps);
  }
  void fill() { fillOrStroke(true); }
  void stroke() { fillOrStroke(false); }
  void dash() {
    ofs << "gsave [1 1] 0 setdash\n";
    dashing = true;
  }
  template <typename... Types>
  void text(double x, double y, Types... args) {
    moveTo(x, y);
    ofs << '(';
    write(args...);
    ofs << ") show\n";
  }

private:
  std::ofstream ofs;
  std::mt19937 rng{6};
  std::uniform_real_distribution<double> col{0, 1};
  bool dashing = false;
  inline void moveTo(double x, double y) { ofs << x << ' ' << y << " moveto\n"; }
  inline void lineTo(double x, double y) { ofs << x << ' ' << y << " lineto\n"; }
  inline void fillOrStroke(bool fill) {
    ofs << (fill ? "fill\n" : "stroke\n");
    if (dashing) {
      dashing = false;
      ofs << "grestore\n";
    }
  }
  template <typename T>
  void write(T x) {
    ofs << x;
  }
  template <typename T, typename... Types>
  void write(T x, Types... args) {
    write(x);
    write(args...);
  }
};
}
}
#endif
// }}}
