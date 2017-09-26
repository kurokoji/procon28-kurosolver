#ifndef PROCON_28_GEO_HPP
#define PROCON_28_GEO_HPP

#include <iostream>
#include <vector>

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/geometry.hpp>

namespace bg = boost::geometry;
namespace trans = bg::strategy::transform;

namespace procon28 {
using Point_Type = long double;                     // x, y座標に使用する型
using Point = bg::model::d2::point_xy<Point_Type>;  // 座標
using Polygon = bg::model::polygon<Point>;          // 図形
using Segment = bg::model::segment<Point>;          // 線分
using Ring = typename Polygon::ring_type;
using InnerContainer = typename Polygon::inner_container_type;
const long double EPS = 1e-10;

std::istream& operator>>(std::istream&, Polygon&);
std::ostream& operator<<(std::ostream&, Polygon&);

const Point operator+(const Point&, const Point&);
const Point operator-(const Point&, const Point&);

Point get_point(const Ring&, int);
Point get_point(const Polygon&, int);
Segment get_segment(const Ring&, int, int);
Segment get_segment(const Polygon&, int, int);
Segment get_segment(const Ring&, int);
Segment get_segment(const Polygon&, int);
long double get_angle(const Segment&);
long double get_angle(const Segment&, const Segment&);
long double get_corner(const Ring&, int);
long double get_corner(const Polygon&, int);
Point to_vec(const Segment&);
Polygon to_frame(const std::vector<Polygon>&);
std::vector<Polygon> to_piece(const Polygon&);
Polygon translate(const Polygon&, Point_Type, Point_Type);
Polygon translate(const Polygon&, Point);
Polygon rotate(const Polygon&, long double);
Polygon rotate(const Polygon&, long double, Point);
Polygon inverse(const Polygon&);
Polygon scale(const Polygon&, double);
void printFrame(const Polygon&, std::ostream&);

}  // namespace procon28

#endif
